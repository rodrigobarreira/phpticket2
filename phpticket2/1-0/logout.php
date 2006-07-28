<?php 
/**************************************   
*** File: logout.php 
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

	require_once('conf.inc');
	require_once('functions01.inc');
	do_logout();
	do_login($GLOBALS['MAIN_PAGE'].'.php');
?>
