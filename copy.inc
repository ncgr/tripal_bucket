<?php

/**
 * Menu callbacks.
 */

/**
 * Submit actions for add2basket link.
 */
function lightshop_add2basket($node, $type) {
  // Verification of the absence of this node in basket.
    $split=split("-", $node);
  if (_in_basket($split[0] )) {
    drupal_set_message(t('"@title" is already in your basket', array('@title' => $split[1])
      ), 'warning');
  }
  else {
    global $user;
    if ($user->uid == 0 && !drupal_session_started()) {
      // Must initialize sessions for anonymous users.
      drupal_session_start();
    }
    $item = array();
    $item['name'] = $split[1];
    if ($item) {
      $_SESSION['basket']['items'][$split[0]] = $item;
      //drupal_set_message(t("'@title' In basket", array('@title' => $split[1])));
    }
  }
  if ($type == 'ajax') {
    $basket_block = lightshop_block_content();
    $commands = array();
    $commands[] = ajax_command_replace(NULL, add2basket_link($split[0]));
    $commands[] = ajax_command_replace('#lightshop-ajax-block-content', render($basket_block));
    $output = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($output);
  }
  else {
    drupal_goto();
  }
}

/**
 * Shared Ajax callback for basket page actions.
 */
function lightshop_ajaxform_process() {
  if (!isset($_SESSION['basket']['items'])) {
    drupal_set_message(t('The form is invalid. Your basket is currently empty.'), 'error');
    return theme('status_messages');
  }
  $commands = array();
  $title = '';

  list($form, $form_state) = ajax_get_form();
  $element_name = $form_state['input']['_triggering_element_name'];

  switch ($element_name) {
    case 'clear':
      $form = drupal_get_form('_lightshop_clear_confirm');
      $form['#action'] = 'basket/clear';
      $title = t('Do you really want to delete all items from the basket?');
      break;

    case 'checkout':
     debug($element_name);
      $form = lightshop_checkout(TRUE);
      $form['#action'] = 'basket/checkout';
      $title = t('IGV');
      break;

    default:
      drupal_process_form($form['#form_id'], $form, $form_state);
  }

  if ($element_name == 'new_order' && !form_get_errors()) {
    $form = array(
      'type' => 'item',
      'value' => node_view($form_state['node']),
    );
    $title = t('Your order has been successfully created');
  }

  $commands[] = ajax_command_replace(NULL, render($form));

  if ($title) {
    $commands[] = ajax_command_html('#lightshop-page-title *:first-child', $title);
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * The form that displays the contents of basket and control buttons.
 */
function lightshop_basket_form($form, &$form_state) {
  // Build Breadcrumbs.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '');
  // Link to current URL.
  $breadcrumb[] = l(drupal_get_title(), current_path());
  // Set Breadcrumbs.
  drupal_set_breadcrumb($breadcrumb);

  // Check for empty session.
  if (!isset($_SESSION['basket']['items'])) {
    drupal_set_title(t('Your basket is empty'));
    $form['empty'] = array(
      '#type' => 'markup',
      '#markup' => t('There is nothing in your basket at the moment.'),
    );
  }
  else {
    $form['#id'] = drupal_html_id('lightshop_basket_form');
    $form['#attributes'] = array(
      'class' => array('lightshop_basket'),
    );
    $ajax_settings = array(
      'path' => 'lightshop/ajax',
      'wrapper' => $form['#id'],
    );

    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear the basket'),
      '#name' => 'clear',
      '#href' => 'bucket/clear',
      #'#ajax' => $ajax_settings,
      '#attributes' => array(
        'class' => array('clear-button'),
      ),
    );

    $form['items'] = array(
      '#tree' => TRUE,
      '#theme' => 'lightshop_items_form',
    );
    foreach ($_SESSION['basket']['items'] as $nid => $data) {
      GLOBAL $url;
      if(strlen($nid)!=0){
       $values = array(
       'biomaterial_id' => $nid
      );
      $features = chado_generate_var('biomaterialprop', $values);
      foreach($features as $f){
        if (preg_match("/bamURL/", $f->type_id->name)){
           if (preg_match("/http/", $f->value)){
              $url=$f->value;
           }
        }
      }
      if(strlen($url)!=0){      
      $form['items'][$nid]['name'] = array(
        '#type' => 'checkbox',
        '#title' => t("<a href=biomaterial/$nid>$data[name]</a>"."(bam))"),
        '#options' => array (
              $url => $data['name']
        ),
      );
     }
     else{
        $form['items'][$nid]['name'] = array(
        '#type' => 'checkbox',
        '#title' => t("<a href=biomaterial/$nid>$data[name]</a>"),
         '#options' => array (
              $url => $data['name']),
       );     
      }
     }
    }

    $form['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array(
        'class' => array('lightshop-form-actions'),
      ),
    );

    $form['actions']['checkout'] = array(
      '#type' => 'submit',
      '#name' => 'checkout',
      '#value' => t('checkout'),
      '#ajax' => $ajax_settings,
      '#attributes' => array(
        'class' => array('checkout-button'),
      ),
    );
    $form['#validate'][] = 'lightshop_check_empty_basket';
    $form['#attached'] = array(
      'css' => array(
        drupal_get_path('module', 'bucket') . '/theme/lightshop.css',
      ),
      'js' => array(
        drupal_get_path('module', 'bucket') . '/theme/lightshop.js',
      ),
    );
  }
  return $form;
}

/**
 * Basket page form qty element validate callback.
 */
function _lightshop_qty_element_validate($element, &$form_state, $form) {
  if (!filter_var($element['#value'], FILTER_VALIDATE_INT) || $element['#value'] <= 0) {
    form_error($element, t('Incorrect value of qty'));
  }
}

/**
 * Basket page form submit callback.
 */
function lightshop_basket_form_submit($form, &$form_state) {

  debug($form_state['triggering_element']['#name']);
  switch ($form_state['triggering_element']['#name']) {
    case 'clear':
      $form_state['redirect'] = 'basket/clear';
      break;

    case 'checkout':
      //drupal_goto('http://localhost/');
      $form_state['redirect'] = 'basket/hello';
      
      break;
  }
}

/**
 * Handling data update form the basket page.
 */
function _lightshop_basket_update($form, &$form_state) {
  $values = $form_state['values']['items'];
  $updated = FALSE;
  foreach ($values as $key => $val) {
    if ($val['del']) {
      drupal_set_message(t('"%title" was removed from your basket', array(
            '%title' => $_SESSION['basket']['items'][$key]['title'],
          )));
      unset($_SESSION['basket']['items'][$key]);
      if (isset($_SESSION['basket']['store'][$key])) {
        unset($_SESSION['basket']['store'][$key]);
      }
      $updated = TRUE;
    }
    elseif (variable_get('lightshop_show_qty', TRUE) && $_SESSION['basket']['items'][$key]['qty'] != $val['qty']) {
      $_SESSION['basket']['items'][$key]['qty'] = $val['qty'];
      if (variable_get('lightshop_show_price', FALSE)) {
        $_SESSION['basket']['items'][$key]['price'] = $val['qty'] * $_SESSION['basket']['store'][$key]['unit_price'];
      }
      drupal_set_message(t('"%title": successfully updated', array(
            '%title' => $_SESSION['basket']['items'][$key]['title'],
          )));
      $updated = TRUE;
    }
  }
  if ($updated) {
    $form_state['rebuild'] = TRUE;
    _lightshop_update_total();
  }
}

/**
 * Updating total sum of basket items.
 */
function _lightshop_update_total() {
  if (empty($_SESSION['basket']['items'])) {
    unset($_SESSION['basket']);
  }
  elseif (variable_get('lightshop_show_price', FALSE) && variable_get('lightshop_price_field', '')) {
    $sum = 0;
    foreach ($_SESSION['basket']['items'] as $item) {
      $sum += $item['price'];
    }
    $_SESSION['basket']['total_sum'] = $sum;
  }
}

/**
 * Calling the checkout form.
 */
function lightshop_checkout($ajax = FALSE) {
  module_load_include('inc', 'node', 'node.pages');
  $add_form = node_add('lightshop_order');
  return $ajax ? $add_form : drupal_render($add_form);
}

/**
 * Confirmation form when cleaning basket.
 */
function _lightshop_clear_confirm($form, &$form_state) {
  if (isset($_SESSION['basket']['items'])) {
    unset($_SESSION['basket']['items']);
    drupal_goto('basket');
    return $form;
     //$form,
     // t('Do you really want to delete all items from the bucket?'),
     // 'bucket',
     // t('This operation is irreversible'),
     // t('Yes, clear'),
     // t('Cancel')
  }
}

/**
 * Confirmation form submit callback.
 */
function _lightshop_clear_confirm_submit($form, &$form_state) {
  unset($_SESSION['basket']['items']);
  drupal_set_message(t('Your basket is cleared'));
  drupal_goto('basket');
}

/**
 * Helper function to check the access code when referring to a specific order.
 */
function _lightshop_get_my_order($node) {
  $code = isset($_GET['c']) && !empty($_GET['c']) ? check_plain($_GET['c']) : FALSE;
  $node_code = isset($node->order_code) ? $node->order_code : FALSE;
  if ($code && $node_code && ($code === $node_code)) {
    $_SESSION['order_codes'][] = $code;
    drupal_goto('node/' . $node->nid);
  }
  drupal_not_found();
}

/**
 * Menu callback; displays page containing recent orders of a given user.
 */
function lightshop_my_orders_page($account) {
  drupal_set_title(t("@name's Order History", array('@name' => format_username($account))), PASS_THROUGH);

  $build = array();
  $query = db_select('node', 'n')->extend('PagerDefault');
  $nids = $query->fields('n', array('nid', 'sticky', 'created'))->condition('type', LIGHTSHOP_ORDER_NODETYPE)->condition('uid', $account->uid)->condition('status', 1)->orderBy('created', 'DESC')->limit(variable_get('default_nodes_main', 10))->addTag('node_access')->execute()->fetchCol();
  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  }
  else {
    drupal_set_message(t('You have no orders for this account.'));
  }

  return $build;
}

