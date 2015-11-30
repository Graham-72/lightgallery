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

    $form['lightgallery']['mode'] = array(
      '#type' => 'select',
      '#title' => t('Type of transition between images'),
      '#options' => _lightgallery_get_modes(),
      '#default_value' => isset($this->options['lightgallery']['mode']) ? $this->options['lightgallery']['mode'] : NULL,
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
