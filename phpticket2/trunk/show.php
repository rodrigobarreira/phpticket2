<?php
/**************************************
*** File: show.php   *****************
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/


require_once('conf.inc');
require_once('functions01.inc');


$page = "show"	  ; // name of this current php page. Use 'index' for 'index.php'.

global $debug;



$auth = True;
session_start();
session_register('auth');
$_SESSION['ticket_username']      = 'anonymous';
$portal_user = $_SESSION['ticket_username'];

$table = $GLOBALS['MAIN_TABLE']; // Table name
$title = $GLOBALS['ELEMENT']	;	  // String identifying the contents
$start_id = "0"; //  Field number to start showing on show_data()

//$element = $GLOBALS['ELEMENT']		 ;
$option_table =$GLOBALS['OPTION_TABLE'];
$option_element =$GLOBALS['OPTION'] ;
$linking_field="ticket_id";
$option_url =$GLOBALS['SECOND_PAGE'] . '.php';

$id=$_GET['id'] ;
if ($id == "" AND $_POST['id'] != "")
{
	$id=$_POST['id'];
}

global $mysql_db_next;
$link_next = mysql_open_next( $mysql_db_next); // Open second Database on MySQL Server

if ($debug) 
{
	print '<p>'.$page.'.php::Start.';
	print '<br>$_GET[email]="'. $_GET['email'] .'"'; 
	print '<br>$id = '.$id;
	print '<br>portal_user = '.$portal_user ;
	print '<br>$_SESSION[user_is_admin] ='.$_SESSION['ticket_user_is_admin'] .'<br><br>';
}


if ($id == '' OR $id == 0)
{

	if ($debug)
	{
		print '<br>'.$page.'.php::$id is empty'  ."\n";
	}
    do_title($title);
    print '<p>Please provide a valid Ticket number';

} else  {
	// Debuggin
	if ($debug)
	{
		print '<br> '.$page.'.php:: id **not** empty::switch()';		
	}
    do_title($title . " #".$id );	 // Set page title
    // Show entry
	print "<!-- Showing the selected Element -->";
	require_once('functions03.inc');
	$i = show_simple_data($id,$start_id);
	$SQL_option= " AND `$linking_field` = $id ";
	mysql_open("$mysql_db", "$mysql_user", "$mysql_passwd");
	//edit_option("", "add", "action",$option_element,$id);
	mysql_open_next("$mysql_db_next", "$mysql_user_next", "$mysql_passwd_next");
	do_title($option_element.'s\' belonging to this '.$GLOBALS['ELEMENT']);
	//list_option($SQL="",$current=0,$search=0)
	//list_option($SQL_option);
	list_simple_option($SQL_option);

}
	print '<hr>';
	//simple_search_by_element_num($GLOBALS['ELEMENT']);
	
powered(); // Print "Powered by" and end the HTML page
session_destroy();
die;
?>
