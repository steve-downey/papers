// fringetree.h                                                       -*-C++-*-
#ifndef INCLUDED_FRINGETREE
#define INCLUDED_FRINGETREE

#include <memory>
#include <variant>
#include <vector>

namespace fringetree {

template <typename Tag, typename Value>
class Branch;

template <typename Tag, typename Value>
class Leaf;

template <typename Tag, typename Value>
class Empty;

template <typename Tag, typename Value>
class Tree;

template <typename Tag, typename Value>
class Branch {
    Tag                               tag_;
    std::shared_ptr<Tree<Tag, Value>> left_;
    std::shared_ptr<Tree<Tag, Value>> right_;

  public:
    Branch() : tag_(0), left_(0), right_(0) {}
    Branch(Tag                               tag,
           std::shared_ptr<Tree<Tag, Value>> left,
           std::shared_ptr<Tree<Tag, Value>> right)
        : tag_(tag), left_(left), right_(right) {}
    auto tag() const -> Tag { return tag_; }
    auto left() const { return left_; }
    auto right() const { return right_; }
};

template <typename Tag, typename Value>
class Leaf {
    Tag   tag_;
    Value v_;

  public:
    Leaf() : tag_(0), v_(0){};
    Leaf(Tag tag, Value v) : tag_(tag), v_(v) {}
    auto tag() const -> Tag { return tag_; }
    auto value() const -> Value { return v_; }
};

template <typename Tag, typename Value>
class Empty {
  public:
    Empty(){};
    auto tag() const -> Tag { return {}; };
};

template <typename Tag, typename Value>
class Tree {
  public:
    using Tag_    = Tag;
    using Value_  = Value;
    using Leaf_   = Leaf<Tag, Value>;
    using Branch_ = Branch<Tag, Value>;
    using Empty_  = Empty<Tag, Value>;

  private:
    std::variant<Empty_, Leaf_, Branch_> data_;

  public:
    Tree(Empty_ const& empty) : data_(empty) {}
    Tree(Leaf_ const& leaf) : data_(leaf) {}
    Tree(Branch_ const& branch) : data_(branch) {}

    auto tag() -> Tag {
        return std::visit([](auto&& v) { return v.tag(); }, data_);
    }

    static auto empty() -> std::shared_ptr<Tree> {
        return std::make_shared<Tree>(Empty_{});
    }

    static auto leaf(Value const& v) -> std::shared_ptr<Tree> {
        return std::make_shared<Tree>(Leaf_{1, v});
    }

    static auto branch(std::shared_ptr<Tree> left, std::shared_ptr<Tree> right)
        -> std::shared_ptr<Tree> {
        return std::make_shared<Tree>(
            Branch_{(left->tag() + right->tag()), left, right});
    }

    template <typename Callable>
    auto visit(Callable&& c) const {
        return std::visit(c, data_);
    }

    bool isEmpty() { return std::holds_alternative<Empty_>(data_); }
};

constexpr auto tag = [](auto tree) { return tree->tag(); };

constexpr inline struct breadth {
    template <typename T, typename V>
    auto operator()(Empty<T, V> const&) const -> T {
        return 0;
    }

    template <typename T, typename V>
    auto operator()(Leaf<T, V> const&) const -> T {
        return 1;
    }

    template <typename T, typename V>
    auto operator()(Branch<T, V> const& b) const -> T {
        return b.left()->visit(*this) + b.right()->visit(*this);
    }
} breadth_;

constexpr auto breadth = [](auto tree) { return tree->visit(breadth_); };

constexpr inline struct depth {
    template <typename T, typename V>
    auto operator()(Empty<T, V> const&) const -> T {
        return 0;
    }

    template <typename T, typename V>
    auto operator()(Leaf<T, V> const&) const -> T {
        return 1;
    }

    template <typename T, typename V>
    auto operator()(Branch<T, V> const& b) const -> T {
        auto leftDepth  = (b.left()->visit(*this)) + 1;
        auto rightDepth = (b.right()->visit(*this)) + 1;

        return (leftDepth > rightDepth) ? leftDepth : rightDepth;
    }
} depth_;

constexpr auto depth = [](auto tree) { return tree->visit(depth_); };

constexpr inline struct flatten {
    template <typename T, typename V>
    auto operator()(Empty<T, V> const&) const -> std::vector<V> {
        return std::vector<V>{};
    }

    template <typename T, typename V>
    auto operator()(Leaf<T, V> const& l) const -> std::vector<V> {
        std::vector<V> v;
        v.emplace_back(l.value());
        return v;
    }

    template <typename T, typename V>
    auto operator()(Branch<T, V> const& b) const -> std::vector<V> {
        auto leftFlatten  = b.left()->visit(*this);
        auto rightFlatten = b.right()->visit(*this);
        leftFlatten.insert(
            leftFlatten.end(), rightFlatten.begin(), rightFlatten.end());
        return leftFlatten;
    }
} flatten_;

constexpr auto flatten = [](auto tree) { return tree->visit(flatten_); };

template <typename OS>
struct printer_ {
    OS& os_;
    printer_(OS& os) : os_(os){};

    template <typename T, typename U>
    void operator()(Empty<T, U> const& e) const {
        os_ << '"' << (&e) << '"' << '\n';
    }

    template <typename T, typename U>
    void operator()(Leaf<T, U> const& l) const {
        os_ << '"' << (&l) << '"'
            << " [shape=record label=\"<f1> value=" << l.value()
            << "\\n tag=" << l.tag() << "\"]\n";
    }

    template <typename T, typename U>
    void operator()(Branch<T, U> const& b) const {
        os_ << '"' << (&b) << '"'
            << " [shape=record label=\"<f0> | <f1> tag=" << b.tag()
            << "| <f2>\" ]\n";
        os_ << '"' << (&b) << "\":f0 -> \"" << (b.left().get()) << "\":f1\n";
        os_ << '"' << (&b) << "\":f2 -> \"" << (b.right().get()) << "\":f1\n";
        (b.left()->visit(*this));
        (b.right()->visit(*this));
    }
};

constexpr auto printer = [](auto& os, auto tree) {
    os << "digraph G {\n";
    printer_ p(os);
    tree->visit(p);
    os << "}\n";
    return;
};

template <typename V>
class prepend_ {
  private:
    V v_;

  public:
    prepend_(V const& v) : v_(v){};
    prepend_(V&& v) : v_(v){};

    template <typename T, typename U>
    auto operator()(Empty<T, U> const&) const -> std::shared_ptr<Tree<T, U>> {
        return Tree<T, U>::leaf(v_);
    }

    template <typename T, typename U>
    auto operator()(Leaf<T, U> const& l) const -> std::shared_ptr<Tree<T, U>> {
        return Tree<T, U>::branch(Tree<T, U>::leaf(v_),
                                  Tree<T, U>::leaf(l.value()));
    }

    template <typename T, typename U>
    auto operator()(Branch<T, U> const& b) const
        -> std::shared_ptr<Tree<T, U>> {
        return Tree<T, U>::branch(Tree<T, U>::leaf(v_),
                                  Tree<T, U>::branch(b.left(), b.right()));
        ;
    }
};

constexpr auto prepend = [](auto v, auto tree) {
    prepend_ p(v);
    return tree->visit(p);
};

template <typename V>
class append_ {
  private:
    V v_;

  public:
    append_(V const& v) : v_(v){};
    append_(V&& v) : v_(v){};

    template <typename T, typename U>
    auto operator()(Empty<T, U> const&) const -> std::shared_ptr<Tree<T, U>> {
        return Tree<T, U>::leaf(v_);
    }

    template <typename T, typename U>
    auto operator()(Leaf<T, U> const& l) const -> std::shared_ptr<Tree<T, U>> {
        return Tree<T, U>::branch(Tree<T, U>::leaf(l.value()),
                                  Tree<T, U>::leaf(v_));
    }

    template <typename T, typename U>
    auto operator()(Branch<T, U> const& b) const
        -> std::shared_ptr<Tree<T, U>> {
        return Tree<T, U>::branch(Tree<T, U>::branch(b.left(), b.right()),
                                  Tree<T, U>::leaf(v_));
        ;
    }
};

constexpr auto append = [](auto v, auto tree) {
    append_ p(v);
    return tree->visit(p);
};

template <typename Tree>
struct View {
  private:
    struct View_ {
        typename Tree::Value_ v_;
        std::shared_ptr<Tree> tree_;
    };

    struct Nil_ {};

    std::variant<View_, Nil_> view_;

  public:
    View(typename Tree::Value_ const& v, std::shared_ptr<Tree> t)
        : view_(View_{v, t}) {}

    View() : view_(Nil_{}) {}

    bool isNil() { return std::holds_alternative<Nil_>(view_); }

    bool isView() { return std::holds_alternative<View_>(view_); }

    auto value() { return std::get<View_>(view_).v_; }
    auto tree() { return std::get<View_>(view_).tree_; }
};

constexpr inline struct view_l {
    template <typename T, typename U>
    auto operator()(Empty<T, U> const&) const -> View<Tree<T, U>> {
        return View<Tree<T, U>>{};
    }

    template <typename T, typename U>
    auto operator()(Leaf<T, U> const& l) const -> View<Tree<T, U>> {
        return View<Tree<T, U>>{l.value(), Tree<T, U>::empty()};
    }

    template <typename T, typename U>
    auto operator()(Branch<T, U> const& b) const -> View<Tree<T, U>> {
        if (b.left()->isEmpty() && b.right()->isEmpty()) {
            return View<Tree<T, U>>{};
        }

        if (b.left()->isEmpty()) {
            return b.right()->visit(*this);
        }

        auto r = b.left()->visit(*this);
        return View<Tree<T, U>>{r.value(),
                                Tree<T, U>::branch(r.tree(), b.right())};
    }
} view_l_;

constexpr auto view_l = [](auto tree) { return tree->visit(view_l_); };

constexpr inline struct view_r {
    template <typename T, typename U>
    auto operator()(Empty<T, U> const&) const -> View<Tree<T, U>> {
        return View<Tree<T, U>>{};
    }

    template <typename T, typename U>
    auto operator()(Leaf<T, U> const& l) const -> View<Tree<T, U>> {
        return View<Tree<T, U>>{l.value(), Tree<T, U>::empty()};
    }

    template <typename T, typename U>
    auto operator()(Branch<T, U> const& b) const -> View<Tree<T, U>> {
        if (b.left()->isEmpty() && b.right()->isEmpty()) {
            return View<Tree<T, U>>{};
        }

        if (b.right()->isEmpty()) {
            return b.left()->visit(*this);
        }

        auto r = b.right()->visit(*this);
        return View<Tree<T, U>>{r.value(),
                                Tree<T, U>::branch(b.left(), r.tree())};
    }
} view_r_;

constexpr auto view_r = [](auto tree) { return tree->visit(view_r_); };

constexpr auto head = [](auto tree) {
    auto view = tree->visit(view_l_);
    return view.value();
};

constexpr auto tail = [](auto tree) {
    auto view = tree->visit(view_l_);
    return view.tree();
};

constexpr auto last = [](auto tree) {
    auto view = tree->visit(view_r_);
    return view.value();
};

constexpr auto init = [](auto tree) {
    auto view = tree->visit(view_r_);
    return view.tree();
};

constexpr auto is_empty = [](auto tree) {
    auto view = tree->visit(view_r_);
    return view.isNil();
};

template <typename T, typename V>
class concat_ {
  private:
    std::shared_ptr<Tree<T, V>> t_;

  public:
    concat_(std::shared_ptr<Tree<T, V>> const& t) : t_(t){};
    concat_(std::shared_ptr<Tree<T, V>>&& t) : t_(t){};

    auto operator()(Empty<T, V> const&) const -> std::shared_ptr<Tree<T, V>> {
        return t_;
    }

    auto operator()(Leaf<T, V> const& leaf) const
        -> std::shared_ptr<Tree<T, V>> {
        auto view = view_l_(leaf);
        return append(view.value(), t_);
    }

    auto operator()(Branch<T, V> const& branch) const
        -> std::shared_ptr<Tree<T, V>> {
        auto    view = view_l_(branch);
        auto    left = append(view.value(), t_);
        concat_ c(left);
        return view.tree()->visit(c);
    }
};

constexpr auto concat = [](auto left, auto right) {
    concat_ c(left);
    return right->visit(c);
};

// ============================================================================
//              INLINE FUNCTION AND FUNCTION TEMPLATE DEFINITIONS
// ============================================================================

} // namespace fringetree

#endif
