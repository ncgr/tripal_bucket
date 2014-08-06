<?php
function bucket_views_data() { 
      $data['example_custom']['table']['group'] = t('example_custom');
      $data['example_custom']['table']['base'] = array( 
           'field' => 'biomaterial_id', // This is the identifier field for the view. 
           'title' => t('Example table'), 
           'help' => t('Example table contains custom content and can be related to nodes.'),
           'weight' => -10, );

      $data['example_custom']['biomaterial_id'] = array( 
           'title' => t('biomaterial_ID'), 
           'help' => t('Some custom content that references a node.'),
           'field' => array( 'handler' => 'views_handler_field_numeric', 'click sortable' => TRUE, ), 
           'filter' => array( 'handler' => 'views_handler_filter_numeric', ), 
           'sort' => array( 'handler' => 'views_handler_sort', ), 
         );


 $data['example_custom']['bio_name'] = array( 
     'title' => t('Sample Name'), 
     'help' => t('Just a plain text field.'),
     'field' => array( 'handler' => 'views_handler_field',  'click sortable' => TRUE,),
     'sort' => array( 'handler' => 'views_handler_sort', ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'argument' => array( 'handler' => 'views_handler_argument_string', ),
     ); 

$data['example_custom']['cultivar'] = array( 
     'title' => t('Cultivar'), 
     'help' => t('Just a string field.'), 
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ), 
     'filter' => array( 'handler' => 'views_handler_filter_string', ), 
     'sort' => array( 'handler' => 'views_handler_sort', ),
     'argument' => array( 'handler' => 'views_handler_argument_string', ),
 ); 

$data['example_custom']['age'] = array(
     'title' => t('Age'),     
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ), 
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );


$data['example_custom']['tissue'] = array(
     'title' => t('Tissue'),     
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ), 
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );

$data['example_custom']['treatment'] = array(
     'title' => t('Treatment'),     
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ), 
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );

$data['example_custom']['phenotype'] = array(
     'title' => t('Phenotype'),     
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ), 
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );

$data['example_custom']['developmental_stage'] = array(
     'title' => t('Development stage'),     
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ), 
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );

$data['example_custom']['source'] = array(
     'title' => t('Source'),     
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ), 
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );
 
 $data['example_custom']['prop_name'] = array(
     'title' => t('Property'),
     'help' => t('Just a string field.'),
     'field' => array( 'handler' => 'views_handler_field', 'click sortable' => TRUE, ),
     'filter' => array( 'handler' => 'views_handler_filter_string', ),
     'sort' => array( 'handler' => 'views_handler_sort', ),
      'argument' => array( 'handler' => 'views_handler_argument_string', ),
 );

     return $data;
 }


function bucket_views_data_alter(&$data) {
  //debug($data['example_custom']);
  $data['example_custom']['actions'] = array(
    'title' => t('Bucket'),
    'help' => t('Clickable links to actions a user may perform on a Node.'),
    'field' => array(
    'handler' => 'bucket_views_handler_field_actions',
    'group' => 'example_custom',
    'click sortable' => FALSE,
    ),
  );
}
 
