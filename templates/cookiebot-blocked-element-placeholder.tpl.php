<?php

/**
 * @file
 * Cookiebot blocked message placeholder for elements like iframes
 * which are disabled via data-src  by cookiebot until consent is given.
 *
 * Available variables:
 * - content: The message content.
 * - attributes: Wrapper attributes.
 * - inner_attributes: Inner wrapper attributes.
 */

$classes = [
  'cookiebot',
  'cookieconsent-optout-marketing'
];
$inner_classes = [
  'cookieconsent-optout-marketing__inner'
];
?>
<div class="<?php print implode(' ', $classes); ?>">
	<div class="<?php print implode(' ', $inner_classes); ?>">
		<?php echo $content; ?>
	</div>
</div>
