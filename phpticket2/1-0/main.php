<?php
/**************************************
*** File: main.php   *****************
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

require_once('conf.inc');
require_once('functions01.inc');
require_once('functions02.inc');
require_once('functions03.inc');

$page=$GLOBALS['MAIN_PAGE']	  ; // name of this current php page. Use 'index' for 'index.php'.
do_login($page .'.php');

$portal_user=$_SESSION['ticket_username'];
global $debug;
$debug = get_variable("debug_value");
$title = $GLOBALS['ELEMENT']	;	  // String identifying the contents
$start_id="0"; //  Field number to start showing on show_data()
$linking_field="ticket_id";
$option_url =$GLOBALS['SECOND_PAGE'] . '.php';
$id=$_GET['id'] ;
if ($id == "" AND $_POST['id'] != "")
{
	$id=$_POST['id'];
}
$aid=$_GET['vid'] ;
$action=$_GET['action'];
global $mysql_db_next;
global $mysql_user_next;
global $mysql_passwd_next;
global $mysql_db ;
global $mysql_user ;
global $mysql_passwd ;

$link_next = mysql_open_next( $mysql_db_next); // Open second Database on MySQL Server

if ($debug)
{
	print '<p>'.$page.'.php::Start.  $id is ' .$id .' .<br>';
	print '<br>$_GET[email]="'. $_GET['email'] .'"';
}

/************************************************
if ($_GET['email'] ==1)
{
	send_email();
	print '<h3>Email has been sent</h3>';
}
************************************************/
if ($id == '')
{
	if ($debug)
	{
		print '<p>'.$page.'.php::$id  is "", $GLOBALS[MAIN_TABLE] is '  . $GLOBALS['MAIN_TABLE'] ;
		print '<br>'.$page.'.php::$id  is "",  $page is '. $page ;
		print '<br>'.$page.'.php::$id  is "",  $title is '. $title ;
		print '<br>'.$page.'.php::$id  is "",  $_GET[SQL] = '. $_GET['SQL']  ."\n";
		print '<br>portal user is "'.$portal_user.'"';
	}
	do_title($title);	 // Set page title

	if ($_GET['SQL'] != " WHERE  1 " && $_GET['SQL']  !="")  // Is there a previous search?
	{
		// Let's strip the variable of inverted slashes.
		$SQL = str_replace("\\","",$_GET['SQL']) ;
		if ($debug)
		{
			print '<p>'.$page.'.php::SQL *not* empty; $SQL is '. $SQL ."\n";
		}
		list_data($GLOBALS['MAIN_TABLE'], $SQL,0,0);//,$element);  // Display resulting data with previous search
	} else {
		list_data($GLOBALS['MAIN_TABLE'], "",0,0,$GLOBALS['ELEMENT']);  // Display resulting data
	}
	if ($debug)
	{
		print '<br>'.$page.'.php::SQL is empty; No $SQL provided '  ."\n";
	}

} else {
	// Debuggin
	if ($debug)
	{
		print '<p> '.$page.'.php:: id not empty::switch()';
		print '<br>$action = '.$action;
		print '<br>$id = '.$id;
		print '<br>portal_user = '.$portal_user ;
		print '<br>$_SESSION[user_is_admin] ='.$_SESSION['ticket_user_is_admin'] .'<br><br>';

	}
	switch ($action)
	{
		case "edit":		// Edit Element
		case "updat": // Update information for Element
		case "delet": // Delete the Element
		case "add":		// Add new Element
			if ($_SESSION['ticket_user_is_admin']>0)
			{
				print "<!-- ".$action."ing the selected Element -->";
				edit_element($id, $action, $GLOBALS['MAIN_TABLE']); //,$page);
			} else {
				do_title(ucfirst($action)."ing ".$GLOBALS['ELEMENT']." "); // . $temp_trailing);
				print '<p class="severity_high">You are not allowed to Add nor Modify existing information';
			}
		break;
		case "own": // take ownership of thit ticket
		case "ownupdat": // update ownership of thit ticket
			own_element($id);
		case "close":		// Close Element
		case "closeupdat":
			if ($_SESSION['ticket_user_is_admin']>0)
			{
				print "<!-- Closing the selected Element -->";
				close_element($id, $action, $GLOBALS['MAIN_TABLE']); //,$page);
			} else {
				do_title(ucfirst($action)."ing ".$GLOBALS['ELEMENT']." "); // . $temp_trailing);
				print '<p class="severity_high">You are not allowed to Add nor Modify existing information';
			}
		break;
		case '':		// Show entry
			print "<!-- Showing the selected Element -->";
			require_once('functions03.inc');
			$i = show_data($id, $GLOBALS['MAIN_TABLE'], $title,$page, $start_id);
			// List Option from current Element
			$SQL_option= " AND `$linking_field` = $id ";
			mysql_open("$mysql_db", "$mysql_user", "$mysql_passwd");
			edit_option("", "add", $id);
			mysql_open_next("$mysql_db_next", "$mysql_user_next", "$mysql_passwd_next");
			do_title($GLOBALS['OPTION']."s belonging to this ".$GLOBALS['ELEMENT'] .". Click on date to edit action", 0 ); // Do not generate HTML headers (the "0")
			list_option($SQL_option);
		break;
	}

}

powered(); // Print "Powered by" and end the HTML page
die;
?>
