<?php 
/**************************************   
*** File: conf.inc.php   **************
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

//Title for this Project. Adapt to your needs.
  global $this_title;
  $this_title = "PHPticket - ";

/****************************************/
/* parameters for mysql, please fill in */
/****************************************/

/* mysql vars for transaction and security */

$mysql_host		= 'localhost'		; 
$mysql_user		= 'youruser'			;
$mysql_passwd	= 'yourpassword'		;
$mysql_db		= 'phpticket_devel'	;

/* Number of lines per page */
global $per_page			;
$per_page	=	20			;

/* Application options */
$GLOBALS['SEND_EMAIL']				="1"; // Set it to '1' for active emails, '0' for no emails
$GLOBALS['SEVERITY_SORT']			="1"; // Set it to '1' for severity sort, '0' for no sort/un-assigned 
$GLOBALS['DEFAULT_UNASSIGNED']		="0"; // Set it to '1' for new tickets marked as "un-assigned
$GLOBALS['DEFAULT_SORT']			= "DESC";
$GLOBALS['SEND_EMAIL_OPTION']		="0"; // Set it to '1' for active emails at new action/option, '0' for no emails

/*******************************************/
/* Thank you for filling in initial values */
/*******************************************/



/****************************************/ 
/* Global Variables for database access */
/****************************************/

/* mysql vars for data */

$mysql_host_next	= $mysql_host	;
$mysql_user_next	= $mysql_user	;
$mysql_passwd_next 	= $mysql_passwd ;
$mysql_db_next		= $mysql_db  	;

global $mysql_db_next		;
global $mysql_user_next		;
global $mysql_passwd_next	;
global $mysql_host_next		;
global $mysql_db_next		;

global $portal_user			;




/* Application constants - do NOT change */
$GLOBALS['STATUS_OPEN']   			= 1;
$GLOBALS['STATUS_CLOSED'] 			= 2;
$GLOBALS['STATUS_WAITING'] 			= 3;
$GLOBALS['CLOSED_FIELD'] 			= "problemend";
$GLOBALS['STATUS_FIELD'] 			= "status";

$GLOBALS['NOTIFY_ACTION'] 			= 'Added Action';
$GLOBALS['NOTIFY_TICKET'] 			= 'Ticket Update';

$GLOBALS['STATUS_UNASSIGNED']   	= 1;
$GLOBALS['STATUS_ASSIGNED'] 		= 2;

$GLOBALS['ACTION_DESCRIPTION']		= 1;
$GLOBALS['ACTION_OPEN'] 			= 2;
$GLOBALS['ACTION_CLOSE'] 			= 3;
$GLOBALS['ACTION_OWNER'] 			= 4;
$GLOBALS['ACTION_PROBLEMSTART'] 	= 5;
$GLOBALS['ACTION_PROBLEMEND'] 		= 6;
$GLOBALS['ACTION_AFFECTED'] 		= 7;
$GLOBALS['ACTION_SCOPE'] 			= 8;
$GLOBALS['ACTION_SEVERITY']			= 9;
$GLOBALS['ACTION_COMMENT']			= 10;

$GLOBALS['SEVERITY_NORMAL'] 		= 0;
$GLOBALS['SEVERITY_MEDIUM'] 		= 1;
$GLOBALS['SEVERITY_HIGH'] 			= 2;

$GLOBALS['MAIN_PAGE']				="main";
$GLOBALS['SECOND_PAGE']				="action";
$GLOBALS['ELEMENT']					="Ticket";
$GLOBALS['OPTION']  				="Action"  ;
$GLOBALS['MAIN_TABLE']				="ticket";
$GLOBALS['OPTION_TABLE']			="action";
$GLOBALS['SELECT_TABLE']			="options";

$GLOBALS['ACTION_FIELDS']			= array(
									"`id` int(8) NOT NULL auto_increment",
									"`ticket_id` int(8) NOT NULL default '0'",
									"`t_date` datetime default NULL",
									"`description` text NOT NULL",
									"`user` int(8) default NULL",
									"`action_type` int(8) default NULL"
									);
$GLOBALS['NOTIFY_FIELDS']			= array(
									"`id` int(8) NOT NULL auto_increment",
									"`ticket_id` int(8) NOT NULL default '0'",
									"`user` int(8) NOT NULL default '0'",
									"`execute_path` tinytext",
									"`on_action` tinyint(1) default '0'",
									"`on_ticket` tinyint(1) default '0'",
									"`email_address` tinytext"
									);
$GLOBALS['OPTIONS_FIELDS']			= array(
									"`id` int(4) NOT NULL auto_increment",
									"`orden` tinyint(4) default NULL",
									"`table_data` varchar(30) NOT NULL default ''",
									"`field` varchar(30) NOT NULL default ''",
									"`value` varchar(30) NOT NULL default ''"
									);
$GLOBALS['SETTINGS_FIELDS']			= array(
									"`id` int(8) NOT NULL auto_increment",
									"`name` tinytext",
									"`value` tinytext",
									"`test` varchar(50) default NULL"								
									);
$GLOBALS['TICKET_FIELDS']			= array(
									"`id` int(8) NOT NULL auto_increment",
									"`t_date` datetime default NULL",
									"`problemstart` datetime default NULL",
									"`problemend` datetime default NULL",
									"`scope` text NOT NULL",
									"`affected` text",
									"`description` text NOT NULL",
									"`status` tinyint(1) NOT NULL default '1'",
									"`owner` tinyint(4) NOT NULL default '1'",
									"`severity` int(2) NOT NULL default '0'",
									"`location` tinyint(4) default NULL"
									);
$GLOBALS['USER_FIELDS']			= array(
									"`id` int(8) NOT NULL auto_increment",
									"`passwd` tinytext",
									"`info` text NOT NULL",
									"`user` tinytext",
									"`admin` tinyint(1) default NULL",
									"`email` tinytext",
									"`ticket_per_page` tinyint(1) default NULL",
									"`sort_desc` tinyint(1) default '0'",
									"`sortorder` tinytext",
									"`reporting` tinyint(1) default '1'"
									);



$add_element_to_table = "ALTER TABLE `settings` ADD `test` VARCHAR( 50 ) ";
									
?>
