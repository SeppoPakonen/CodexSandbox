/**
 * Pearl Page animation script
 * Site-wide JS effects are loaded from here
 * @version 0.6
*/

function initPageAnims() {

    // Set up the image caption fade in/out effect, but only
    // if the element is hidden by default
    if ( $('div.imgcaption div').css('display') != 'block' ) {
        
        $('div.imgcaption').hover(function () {
            $('div', this).stop(true, true).fadeIn(250);
        }, function () {
            $('div', this).fadeOut(250);
        });
    }
    
    $('input.text[title]').attr('value', function () {
        return $(this).attr('title');
    });
    var input_text_color = $('input.text[title]').css('color');
    $('input.text[title]').css('color', '#999');
    $('input.text[title]').focus(function () {
        if ($(this).css('color') != input_text_color) {
            $(this).attr('value', '');
            $(this).css('color', input_text_color);
        }
    });
    $('input.text[title]').blur(function () {
        if ($(this).attr('value') == '') {
            $(this).attr('value', $(this).attr('title'));
            $(this).css('color', '#999');
        }
    });

}
$(document).ready(function () {
    initPageAnims();
});