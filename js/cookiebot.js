/**
 * @file
 * Cookiebot functionality.
 */

(function ($, Drupal) {
  'use strict';

  /**
   * Attach Cookiebot renew click event listener.
   *
   * @type {Drupal~behavior}
   */
  Drupal.behaviors.cookiebot = {
    attach: function attach(context) {
      $('.cookiebot-renew', context).once().on('click', function (event) {
        if (typeof Cookiebot === 'undefined') {
          return;
        }

        Cookiebot.renew();
        event.preventDefault();
      });
    }
  };

  /**
   * Updates cookies for Cookiebot.
   *
   * We set our own cookies to be able to provide integration with other Drupal
   * modules, without relying on the cookies of Cookiebot, since those are not
   * part of the public API.
   */
  Drupal.cookiebot = {
    updateCookies: function updateCookies() {
      var cookieNames = [
        'necessary',
        'preferences',
        'statistics',
        'marketing'
      ];

      $.each(cookieNames, function (index, cookieName) {
        if (Cookiebot.consent[cookieName] === true) {
          jQuery.cookie('cookiebot-consent--' + cookieName, 1);
          return;
        }

        jQuery.cookie('cookiebot-consent--' + cookieName, 0);
      });
    }
  };

})(jQuery, Drupal);

/* eslint-disable no-unused-vars, strict */

/**
 * The asynchronous callback when the user accepts the use of cookies.
 *
 * This is also called on every page load when cookies are already accepted.
 */
function CookiebotCallback_OnAccept() {
  Drupal.cookiebot.updateCookies();
}

/**
 * The asynchronous callback when the user declines the use of cookies.
 *
 * This is also called on every page load when cookies are already declined.
 */
function CookiebotCallback_OnDecline() {
  Drupal.cookiebot.updateCookies();
}

/* eslint-enable no-unused-vars, strict */
