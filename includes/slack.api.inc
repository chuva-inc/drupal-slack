<?php

/**
 * @file
 * Slack integration module API functions.
 */

/**
 * Send message to the Slack.
 *
 * @param string $message
 *   The message sent to the channel
 * @param string $channel
 *   The channel in the Slack service to send messages
 * @param string $username
 *   The bot name displayed in the channel
 *
 * @return bool|object
 *   Slack response.
 */
function slack_send_message($message, $channel = '', $username = '') {
  $webhook_url = slack_get_default_webhook_url();
  if (!$webhook_url) {
    return FALSE;
  }
  $message_options = array();
  if ($channel) {
    $message_options['channel'] = $channel;
  }
  else {
    $message_options['channel'] = slack_get_default_channel();
  }
  if ($username) {
    $message_options['username'] = $username;
  }
  else {
    $message_options['username'] = slack_get_default_username();
  }
  $result = _slack_send_message($webhook_url, $message, $message_options);
  return $result;
}

/**
 * Send message to the Slack with more options.
 *
 * @param string $team_name
 *   Your team name in the Slack
 * @param string $team_token
 *   The token from "Incoming WebHooks" integration in the Slack
 * @param string $message
 *   The message sent to the channel
 * @param array $message_options
 *   An associative array, it can contain:
 *     - channel: The channel in the Slack service to send messages
 *     - username: The bot name displayed in the channel
 * @param string $parse_mode
 *   Sets the parsing mode for the encapsuled message, see
 *     https://api.slack.com/docs/formatting#parsing_modes
 *   Supported values are:
 *     - 'none' (default): the message will be posted as-is
 *     - 'full': any text field (including attachments) will be post-processed
 *         to include @user and links.
 *
 * @return
 *   See _slack_http_request().
 */
function _slack_send_message($webhook_url, $message, $message_options = array(), $parse_mode = 'none') {
  $message_options['text'] = $message;

  return _slack_http_request($webhook_url, $message, $message_options, $parse_mode);
}

/**
 * Wrapper for drupal_http_request().
 *
 * @return object
 *   Can contain:
 *                          success      fail          fail
 *     - data:                ok         No hooks      Invalid channel specified
 *     - status message:      OK         Not found     Server Error
 *     - code:                200        404           500
 *     - error:               -          Not found     Server Error
 */
function _slack_http_request($webhook_url, $message_options = array(), $parse_mode = 'none') {
  $headers = array(
    'Content-Type' => 'application/x-www-form-urlencoded',
  );

  $sending_data = 'parse=' . $parse_mode . '&payload=' . drupal_json_encode($message_options);
  $options = array(
    'method' => 'POST',
    'data' => $sending_data,
    'headers' => $headers,
  );
  $sending_url = $webhook_url;
  $result = drupal_http_request($sending_url, $options);
  return $result;
}

/**
 * Get default Webhook URL.
 *
 * @return string
 *   Get default Webhook URL.
 */
function slack_get_default_webhook_url() {
  $channel = variable_get('slack_webhook_url');
  return $channel;
}

/**
 * Get default team channel.
 *
 * @return string
 *   Get default team channel
 */
function slack_get_default_channel() {
  $channel = variable_get('slack_channel');
  return $channel;
}

/**
 * Get default Slack bot username.
 *
 * @return string
 *   Get default Slack bot username
 */
function slack_get_default_username() {
  $username = variable_get('slack_username');
  return $username;
}
