<?php

/**
 * @file
 * handler class for the new field 'bucket'
 */

/**
 * class definition
 */
class bucket_views_handler_field_actions extends views_handler_field {
  /**
   * Render function: return html output
  */
  function render($values) {
    // debug ($values);
     //$biomaterial_id=$values->biomaterial_id;
     return add2basket_link($values);
   }
  function query() {
    // Do nothing, leave query blank, we render the contents
  }

}
