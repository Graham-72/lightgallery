/**
 * @file
 * JS to init light gallery.
 */

(function ($) {
  Drupal.behaviors.lighgallery = {
    galleries: [],
    attach: function (context, settings) {
      var that = this;
      // Open gallery on "show more" click.
      $('.lightgallery-wrapper a.show-more').click(function (e) {
        e.preventDefault();

        var id = $(this).attr('data-unique-id');
        var slide =  $(this).attr('data-open-slide');
        $('#lightgallery-' + id, context).data('lightGallery').build(parseInt(slide));

        return false;
      });

      // Init all galleries.
      for (id in settings.lightgallery.instances) {
        // Store galleries so that developers can change options.
        that.galleries[id] = settings.lightgallery.instances[id];
        _lightgallery_init(id, that.galleries[id], context);
      }
    }
  };

  function _lightgallery_init(id, optionset, context) {
    $('#lightgallery-' + id, context).lightGallery(optionset);
  }

})(jQuery);
