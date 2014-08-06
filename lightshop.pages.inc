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
  GLOBAL $hash;
  // Check for empty session.
  if (!isset($_SESSION['basket']['items'])) {
    drupal_set_title(t('Your basket is empty'));
    $form['empty'] = array(
      '#type' => 'markup',
      '#markup' => t('There is nothing in your basket at the moment.'),
    );
  }
  else {  
    $count=0;
    foreach ($_SESSION['basket']['items'] as $nid => $data) {
      GLOBAL $url;
      if(strlen($nid)!=0){
       $values = array(
       'biomaterial_id' => $nid
      );
      $features = chado_generate_var('biomaterialprop', $values);
      if (preg_match("/http/", $features->value)){
         $url=$features->value;
      }
      if(strlen($url)!=0){      
        $hash[$url]="<a href=?q=biomaterial/$nid>$data[name]</a>". " [BAM] ";
     }
     else{
         ++$count;
         $hash[$count]="<a href=?q=biomaterial/$nid>$data[name]</a>";     
      }
     }
    }
   $form['items'] = array(
      '#title' => t('Bucket'),
      '#type' => 'fieldset',
  );  

   $form['items']['name'] = array(
      '#title' => t('Select'),
      '#type' => 'checkboxes',
      '#description' => t('Select and submit to launch IGV.'),
       '#options' => $hash,
  );

  $form['submit'] = array (
  '#type' => 'submit',
  '#value' => t('IGV'),
  '#name' => 'igv',
  );

   $form['reset'] = array (
  '#type' => 'submit',
  '#value' => t('Reset'),
  '#name' => 'reset',
  );

 
  $form['clear'] = array (
    '#type' => 'submit',
   '#value' => t('Clear bucket?'),
    '#name' => 'clear',
    ); 
  }
return $form;
}

/**
 * Basket page form submit callback.
 */

function lightshop_basket_form_submit($form, &$form_state) {
    $concat; 

    switch ($form_state['triggering_element']['#name']) {
    case 'clear':
      $form_state['redirect'] = 'basket/clear';
      break;

    case 'reset':
    $form['items']['name'] = array(
    '#type' => 'markup',
    '#value' => '<input class="form-button" value="Reset" type="reset">',
    '#weight' => 2000,
    );
    break;

    case 'igv':
      $values=$form_state['complete form']['items']['name']['#value'];
      $check=0;
      foreach($values as $key){
       if(preg_match("/http/", $key)){
       $concat.="$key,";
       }
       else{
          ++$check;
        }
      }
      substr_replace($concat, ",", -1);
      if($check!=0){
        drupal_set_message(t('Error: One or more selections do not include BAM'));
      }
      else{
         $form_state['redirect'] = "http://localhost:60151/load?file=$concat";
         break;
      }
    }
}

/**
 * Confirmation form when cleaning basket.
 */
function _lightshop_clear_confirm($form, &$form_state) {
  if (isset($_SESSION['basket']['items'])) {
    unset($_SESSION['basket']['items']);
    drupal_set_message(t('Your basket is cleared!'));
    drupal_goto('basket');
    return $form;
  }
}

/**
 * Confirmation form submit callback.
 */
//function _lightshop_clear_confirm_submit($form, &$form_state) {
//  unset($_SESSION['basket']['items']);
 // drupal_set_message(t('Your basket is cleared'));
 // drupal_goto('basket');
//}



