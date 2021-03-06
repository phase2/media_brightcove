<?php

/**
 *  @file
 *  Create a Brightcove Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $youtube = new ResourceBrightcoveStreamWrapper('youtube://?v=[video-code]');
 */
class MediaBrightcoveStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://brightcove.com/watch';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/brightcove';
  }

  function getOriginalThumbnailPath() {
    $params = $this->get_parameters();
    $video = brightcove_video_load($params['v']);
    return $video->videoStillURL;

    return 'http://img.brightcove.com/vi/'. check_plain($parts['v']) .'/0.jpg';
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-brightcove/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}
