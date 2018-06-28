/**
 * @file
 * Cookiebot functionality.
 */

(function ($, Drupal) {
  'use strict';

  var $window = $(window);
  var renewConsentTriggerEventName = 'cookiebotConsentRenew';

  /**
   * Capitalize the first character of a given string.
   *
   * @param {string} string
   *   The string to capitalize the first character of.
   *
   * @return {string}
   *   The string with the first letter capitalized.
   */
  var capitalizeFirstCharacter = function (string) {
    return string[0].toUpperCase() + string.substr(1);
  };

  $window.on(renewConsentTriggerEventName, function () {
    if (typeof Cookiebot === 'undefined') {
      return;
    }

    Cookiebot.renew();
  });

  /**
   * Attach Cookiebot renew click event listener.
   *
   * @type {Drupal~behavior}
   */
  Drupal.behaviors.cookiebot = {
    attach: function attach(context) {
      $('.cookiebot-renew', context).once().on('click', function (event) {
        event.preventDefault();

        $window.trigger(renewConsentTriggerEventName);
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
        if (Cookiebot.consent[cookieName] === true && $.cookie('cookiebot-consent--' + cookieName) !== '1') {
          $.cookie('cookiebot-consent--' + cookieName, '1', {
            path: '/'
          });
          $window.trigger('cookiebotConsentAccept' + capitalizeFirstCharacter(cookieName));
          return;
        }

        if (Cookiebot.consent[cookieName] === false && $.cookie('cookiebot-consent--' + cookieName) !== '0') {
          $.cookie('cookiebot-consent--' + cookieName, '0', {
            path: '/'
          });
          $window.trigger('cookiebotConsentDecline' + capitalizeFirstCharacter(cookieName));
        }
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
