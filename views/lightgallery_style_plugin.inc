<?php

/**
 * @file
 * Contains the lightgallery style plugin.
 */

/**
 * Style plugin to render views as lightgallery instance.
 *
 * @ingroup views_style_plugins
 */
class lightgallery_style_plugin extends views_plugin_style {

  /**
   * Provide default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['lightgallery'] = array('default' => array());

    return $options;
  }

  /**
   * Adds Lightgallery configuration form options.
   */
  function options_form(&$form, &$form_state) {
    // Check if fields have been added.  Table style plugin will set
    // error_markup if fields are not added.
    // @see views_plugin_style::options_form()
    if (isset($form['error_markup'])) {
      return;
    }

    $fields = $this->conf_get_field_sources();
    $missing_field_warning = '';
    if (empty($fields['field_options_images'])) {
      $missing_field_warning = t('<strong>You must add a field of type image, file or file ID to your view display before this value can be set.</strong><br/>');
    }

    $form['lightgallery'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lightgallery settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['lightgallery']['thumb_field'] = array(
      '#title' => t('Thumbnail field'),
      '#type' => 'select',
      '#options' => $fields['field_options_images'],
      '#default_value' => isset($this->options['lightgallery']['thumb_field']) ? $this->options['lightgallery']['thumb_field'] : NULL,
      '#description' => t('Select the field you want to use to display the thumbnails on page load.'),
      '#suffix' => $missing_field_warning,
      '#required' => TRUE,
    );

    $form['lightgallery']['image_field'] = array(
      '#title' => t('Image field'),
      '#type' => 'select',
      '#options' => $fields['field_options_images'],
      '#default_value' => isset($this->options['lightgallery']['image_field']) ? $this->options['lightgallery']['image_field'] : NULL,
      '#description' => t('Select the field you want to use to display the images in the Lightgallery.'),
      '#suffix' => $missing_field_warning,
      '#required' => TRUE,
    );

    $form['lightgallery']['title'] = array(
      '#title' => t('Title field'),
      '#type' => 'select',
      '#options' => array('' => t('None')) + $fields['field_options'],
      '#default_value' => isset($this->options['lightgallery']['title']) ? $this->options['lightgallery']['title'] : NULL,
      '#description' => t('Select the field you want to use as title in the Lightgallery. Leave empty to omit titles.'),
      '#required' => FALSE,
    );

    $form['lightgallery']['mode'] = array(
      '#type' => 'select',
      '#title' => t('Transition'),
      '#options' => _lightgallery_get_modes(),
      '#default_value' => isset($this->options['lightgallery']['mode']) ? $this->options['lightgallery']['mode'] : NULL,
      '#description' => t('Type of transition between images.'),
    );

    $form['lightgallery']['preload'] = array(
      '#type' => 'select',
      '#title' => t('Preload'),
      '#options' => drupal_map_assoc(array(1, 2, 3, 4)),
      '#default_value' => isset($this->options['lightgallery']['preload']) ? $this->options['lightgallery']['preload'] : 1,
      '#description' => t('number of preload slides. will exicute only after the current slide is fully loaded. '
          . 'ex:// you clicked on 4th image and if preload = 1 then 3rd slide and 5th slide will be loaded in the background after the 4th slide is fully loaded. '
          . 'If preload is 2 then 2nd 3rd 5th 6th slides will be preloaded'),
    );

    $form['lightgallery']['thumbnails'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use thumbnails'),
      '#default_value' => isset($this->options['lightgallery']['thumbnails']) ? $this->options['lightgallery']['thumbnails'] : TRUE,
      '#description' => t('Indicate if you want to use thumbnails in the LightGallery.')
    );

    $form['lightgallery']['closable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Closable'),
      '#default_value' => isset($this->options['lightgallery']['closable']) ? $this->options['lightgallery']['closable'] : TRUE,
      '#description' => t('Allows clicks on dimmer to close gallery.')
    );

    $form['lightgallery']['loop'] = array(
      '#type' => 'checkbox',
      '#title' => t('Loop'),
      '#default_value' => isset($this->options['lightgallery']['loop']) ? $this->options['lightgallery']['loop'] : TRUE,
      '#description' => t('If not selected, the ability to loop back to the beginning of the gallery when on the last element, will be disabled.')
    );

    $form['lightgallery']['esc_key'] = array(
      '#type' => 'checkbox',
      '#title' => t('Escape key'),
      '#default_value' => isset($this->options['lightgallery']['esc_key']) ? $this->options['lightgallery']['esc_key'] : TRUE,
      '#description' => t('Whether the LightGallery could be closed by pressing the "Esc" key.')
    );

    $form['lightgallery']['key_press'] = array(
      '#type' => 'checkbox',
      '#title' => t('Keyboard'),
      '#default_value' => isset($this->options['lightgallery']['key_press']) ? $this->options['lightgallery']['key_press'] : TRUE,
      '#description' => t('Enable keyboard navigation.')
    );

    $form['lightgallery']['controls'] = array(
      '#type' => 'checkbox',
      '#title' => t('Controls'),
      '#default_value' => isset($this->options['lightgallery']['controls']) ? $this->options['lightgallery']['controls'] : TRUE,
      '#description' => t('If not checked, prev/next buttons will not be displayed.')
    );

    $form['lightgallery']['mouse_wheel'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mouse wheel'),
      '#default_value' => isset($this->options['lightgallery']['mouse_wheel']) ? $this->options['lightgallery']['mouse_wheel'] : TRUE,
      '#description' => t('Chane slide on mousewheel')
    );

    $form['lightgallery']['download'] = array(
      '#type' => 'checkbox',
      '#title' => t('Download'),
      '#default_value' => isset($this->options['lightgallery']['download']) ? $this->options['lightgallery']['download'] : TRUE,
      '#description' => t('Enable download button. '
          . 'By default download url will be taken from data-src/href attribute but it supports only for modern browsers.'),
    );

    $form['lightgallery']['counter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Counter'),
      '#default_value' => isset($this->options['lightgallery']['counter']) ? $this->options['lightgallery']['counter'] : TRUE,
      '#description' => t('Whether to show total number of images and index number of currently displayed image.'),
    );

    $form['lightgallery']['drag'] = array(
      '#type' => 'checkbox',
      '#title' => t('Drag'),
      '#default_value' => isset($this->options['lightgallery']['drag']) ? $this->options['lightgallery']['drag'] : TRUE,
      '#description' => t('Enables desktop mouse drag support.'),
    );

    $form['lightgallery']['touch'] = array(
      '#type' => 'checkbox',
      '#title' => t('Touch'),
      '#default_value' => isset($this->options['lightgallery']['touch']) ? $this->options['lightgallery']['touch'] : TRUE,
      '#description' => t('Enables touch support.'),
    );

    $form['lightgallery']['full_screen'] = array(
      '#type' => 'checkbox',
      '#title' => t('Full screen'),
      '#default_value' => isset($this->options['lightgallery']['full_screen']) ? $this->options['lightgallery']['full_screen'] : TRUE,
      '#description' => t('Enable/Disable fullscreen mode.'),
    );

    $form['lightgallery']['zoom'] = array(
      '#type' => 'checkbox',
      '#title' => t('Zoom'),
      '#default_value' => isset($this->options['lightgallery']['zoom']) ? $this->options['lightgallery']['zoom'] : TRUE,
      '#description' => t('Enable/Disable zoom option.'),
    );

    $form['lightgallery']['hash'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update hash in url'),
      '#default_value' => isset($this->options['lightgallery']['hash']) ? $this->options['lightgallery']['hash'] : TRUE,
      '#description' => t('Enable/Disable hash option.'),
    );
  }

  /**
   * Add form validation for options.
   */
  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    // Form_error($form['lightgallery']['height'], 'test');
  }

  function validate() {
    parent::validate();
    $errors = array();
    // Make sure the pager is not enabled.
    if ($this->display->handler->use_pager()) {
      $errors[] = t('The Lighgallery style cannot be used with a pager. Please disable the "Use pager" option for this display.');
    }

    return $errors;
  }

  /**
   * Renders the view fields.
   * @param type $result
   * @return type
   */
  function render_fields($result) {
    $this->rendered_fields = array();
    $this->view->row_index = 0;
    $keys = array_keys($this->view->field);

    // If all fields have a field::access FALSE there might be no fields, so
    // there is no reason to execute this code.
    if (!empty($keys)) {
      $fields = $this->conf_get_field_sources();
      $image_fields = array_keys($fields['field_options_images']);
      foreach ($result as $count => $row) {
        $this->view->row_index = $count;
        foreach ($keys as $id) {
          if (in_array($id, $image_fields)) {
            // This is an image/thumb field.
            // Create URI for selected image style.
            $image_style = $this->view->field[$id]->options['settings']['image_style'];
            $uri = !empty($result[$count]->{'field_' . $id}[0]['raw']['uri']) ? $result[$count]->{'field_' . $id}[0]['raw']['uri'] : FALSE;
            if ($uri) {
              if (!empty($image_style)) {
                $this->rendered_fields[$count][$id] = file_create_url(image_style_url($image_style,
                        $uri));
              }
              else {
                $this->rendered_fields[$count][$id] = file_create_url($uri);
              }
            }
          }
          else {
            // Just render the field as views would do.
            $this->rendered_fields[$count][$id] = $this->view->field[$id]->theme($row);
          }
        }

        $this->row_tokens[$count] = $this->view->field[$id]->get_render_tokens(array());
      }
    }
    unset($this->view->row_index);


    return $this->rendered_fields;
  }

  /**
   * Fetches field options for all fields definied in view.
   * @return type
   */
  private function conf_get_field_sources() {
    $view = $this->view;
    $options = array(
      'field_options_images' => array(),
      'field_options_images_type' => array(),
      'field_options' => array(),
    );
    $field_handlers = $this->display->handler->get_handlers('field');
    if (!empty($view->base_table) && $view->base_table == 'file_managed') {
      $options['field_options_images_type']['file_base'] = 'file_base';
    }
    $field_labels = $this->display->handler->get_field_labels();
    // Get the label for each field.
    foreach ($field_handlers as $field => $handler) {
      $name = $field_labels[$field];
      // Separate image fields from non-image fields. For image fields we can
      // work with fids and fields of type image or file.
      if (isset($handler->field) && $handler->field == 'fid') {
        $options['field_options_images'][$field] = $name;
        $options['field_options_images_type'][$field] = 'file_id_field';
      }
      elseif (isset($handler->field_info['type']) && ($handler->field_info['type'] == 'image' || $handler->field_info['type'] == 'file')) {
        $options['field_options_images'][$field] = $name;
        $options['field_options_images_type'][$field] = 'file_field';
      }
      else {
        $options['field_options'][$field] = $name;
      }
    }
    return $options;
  }

}
