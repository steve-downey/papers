(setq org-re-reveal-postamble
"<style type=\"text/css\">
    /* 1. Style header/footer <div> so they are positioned as desired. */
    #header-left {
        position: absolute;
        top: 0%%;
        left: 0%%;
    }
    #header-right {
        position: absolute;
        top: 0%%;
        right: 0%%;
    }
    #footer-left {
        position: absolute;
        bottom: 0%%;
        left: 0%%;
        font-size: 0.5em;
    }
</style>

<!-- 2. Create hidden header/footer <div> -->
<div id=\"hidden\" style=\"display:none;\">
<div id=\"header\">
<div id=\"footer-left\">© 2023 Bloomberg Finance L.P. All rights reserved</div>
</div>
</div>

<script src=\"https://code.jquery.com/jquery-2.2.4.min.js\"></script>
<script type=\"text/javascript\">
// 3. On Reveal.js ready event, copy header/footer <div> into each `.slide-background` <div>
var header = $('#header').html();
if ( window.location.search.match( /print-pdf/gi ) ) {
Reveal.addEventListener( 'ready', function( event ) {
                                  $('.slide-background').append(header);
                                  });
}
else {
$('div.reveal').append(header);
}
</script>
")
