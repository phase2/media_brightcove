<?php

/**
 * @file
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_brightcove_file_formatter_info() {
  $formatters['media_brightcove_video'] = array(
    'label' => t('Brightcove Video'),
    'file types' => array('video'),
    'default settings' => array(
      'player' => '',
    ),
    'view callback' => 'media_brightcove_file_formatter_video_view',
    'settings callback' => 'media_brightcove_file_formatter_video_settings',
  );
  $formatters['media_brightcove_image'] = array(
    'label' => t('Brightcove Preview Image'),
    'file types' => array('video'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_brightcove_file_formatter_image_view',
    'settings callback' => 'media_brightcove_file_formatter_image_settings',
  );
  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_brightcove_file_formatter_video_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme == 'brightcove') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $params = $wrapper->get_parameters();
    $element = array(
      '#theme' => 'brightcove_field_embed',
      '#video_id' => $params['v'],
    );
    if ($display['settings']['player_id']) {
      $element['#player_id'] = $display['settings']['player_id'];
      $element['#player_key'] = $display['settings']['player_key'];
    }
    drupal_add_js('http://admin.brightcove.com/js/BrightcoveExperiences.js');
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_brightcove_file_formatter_video_settings($form, &$form_state, $settings) {
  $element = array();
  $element['player_id'] = array(
    '#title' => t('Player Id'),
    '#type' => 'textfield',
    '#default_value' => $settings['player_id'],
  );
  $element['player_key'] = array(
    '#title' => t('Player Key'),
    '#type' => 'textfield',
    '#default_value' => $settings['player_key'],
  );
  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_brightcove_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme == 'brightcove') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $image_style = $display['settings']['image_style'];
    $valid_image_styles = image_style_options(FALSE);
    if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
      $element = array(
        '#theme' => 'image',
        '#path' => $wrapper->getOriginalThumbnailPath(),
      );
    }
    else {
      $element = array(
        '#theme' => 'image_style',
        '#style_name' => $image_style,
        '#path' => $wrapper->getLocalThumbnailPath(),
      );
    }
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_brightcove_file_formatter_image_settings($form, &$form_state, $settings) {
  $element = array();
  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );
  return $element;
}

/**
 * Implements hook_file_default_displays().
 */
function media_brightcove_file_default_displays() {
  $default_displays = array();
  return $default_displays;

  // Default settings for displaying as a video.
  $default_video_settings = array(
    'media_large' => array(
      'width' => 480,
      'height' => 360,
      'autoplay' => FALSE,
    ),
    'media_original' => array(
      'width' => 640,
      'height' => 480,
      'autoplay' => media_brightcove_variable_get('autoplay'),
    ),
  );
  foreach ($default_video_settings as $view_mode => $settings) {
    $display_name = 'video__' . $view_mode . '__media_brightcove_video';
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 1,
      'settings' => $settings,
    );
  }

  // Default settings for displaying a video preview image. We enable preview
  // images even for view modes that also play video, for use inside a running
  // WYSIWYG editor. The higher weight ensures that the video display is used
  // where possible.
  $default_image_styles = array(
    'media_preview' => 'square_thumbnail',
    'media_large' => 'large',
    'media_original' => ''
  );
  foreach ($default_image_styles as $view_mode => $image_style) {
    $display_name = 'video__' . $view_mode . '__media_brightcove_image';
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 2,
      'settings' => array('image_style' => $image_style),
    );
  }

  return $default_displays;
}
