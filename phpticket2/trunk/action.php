<?php
/**************************************
*** File: action.php   *****************
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


$page= $GLOBALS['SECOND_PAGE']	; // name of this current php page. Use 'index' for 'index.php'.
do_login($page .'.php');

//$element_page=$GLOBALS['MAIN_PAGE'];

//$portal_user=$_SESSION['ticket_username'];
global $debug;
$debug = get_variable("debug_value");
//$table=$GLOBALS['OPTION_TABLE']; // Table name
$title = $GLOBALS['OPTION']  ;  // String identifying the contents
//$option  = $GLOBALS['OPTION']    ;
$element_table =$GLOBALS['MAIN_TABLE'];
//global $element;
//$main_element =$element;
$start_id="1"; //  Field number to start showing on show_data()
//global $option; // What are we dealing with: AE Tiles, Servers,
$id=$_GET['id'] ;
$action=$_GET['action'];
$option_id =$_GET['element_id'];
global $mysql_db_next;
$link_next = mysql_open_next( $mysql_db_next); // Open second Database on MySQL Server
if ($debug)
{
	print '<p>action.php::action() $id is '  . $id ;
	print '<br>action.php::action() $table is '  . $GLOBALS['OPTION_TABLE'] ;
	print '<br>action.php::action() $element_id is '  . $element_id ;
	print '<br>action.php::action()  $id is ' .$id .' .<br>'."\n";

}
if ($id == '')
{
	if ($debug)
	{
		print '<p>action.php::action() $table is '  . $GLOBALS['OPTION_TABLE'] ;
		print '<br>action.php::action() $page is '. $page ;
		print '<br>action.php::action() $title is '. $title ;
		print '<br>action.php::action() \$_GET[SQL] = '. $_GET['SQL']  ;
	}
	do_title($title);	 // Set page title

	if ($_GET['SQL'] != " WHERE  1 " && $_GET['SQL']  !="")  // Is there a previous search?
	{
		// Let's strip the variable of inverted slashes.
		$SQL = str_replace("\\","",$_GET['SQL']) ;
		if ($debug)
		{
			print '<p>action.php::action() $SQL is '. $SQL ;
		}
		list_option( $SQL);  // Display resulting data with previous search
	}
	else
	{
		list_option();  // Display resulting data
	}
	if ($debug)
	{
		print '<p>action.php::action() No $SQL provided ' ;
	}
}
else
{
	if ($action == "updat") { 
		//header("Refresh: 3; main.php?id=".$element_id);
		set_htmlheader(ucfirst($action)."ing ".$title." " . $temp_trailing, 3, "main.php?id=".$element_id);
	} else {
		set_htmlheader(ucfirst($action)."ing ".$title." " . $temp_trailing);
	}
	// Debuggin
	if ($debug)
	{
		print '<br>action.php::switch()::$id = '.$id."\n";
		print '<br>action.php::switch()::$element_id = '.$element_id."\n";
		print '<br>action.php::switch()::$action = '.$action."\n";
		print '<br>action.php::switch()::$table = '.$GLOBALS['OPTION_TABLE']."\n";
		print '<br>action.php::switch()::$page = '.$page."\n";
		print '<br>action.php::switch()::$option = '.$GLOBALS['OPTION']."\n";
		print '<br>action.php::switch()::$portal_user = '.$_SESSION['ticket_username'] ."\n";
		print '<br>user_is_admin = '.$_SESSION['ticket_user_is_admin']."\n";
	}
	switch ($action)
	{
		case "updat": // Update information for Element
			//if ($action == "updat") { header("Refresh: 3; main.php?id=".$element_id); }
		case "edit":  // Edit Element
		case "delet": // Delete the Element
		case "add":		// Add new Element
			if ($_SESSION['ticket_user_is_admin']>0)
			{
				print "<!-- ".$action."ing the selected Element -->";
				// edit_option($id,$action,$page,$GLOBALS['OPTION'], $element_id="")
				edit_option($id, $action, $page,$element_id);
			}
			else
			{
				do_title(ucfirst($action)."ing ".$title." " . $temp_trailing);
				print '<p class="severity_high">You are not allowed to Add nor Modify existing information';
			}
		break;
		case '':		// Show entry
			print "<!-- Showing the selected Element -->";
			//  function show_action_data($id,$table,$start_id=0)
			$i = show_action_data($id, $GLOBALS['OPTION_TABLE'], $title,$start_id, $GLOBALS['MAIN_PAGE']);
		break;
	}
}

powered(); // Print "Powered by" and end the HTML page
die;
?>
