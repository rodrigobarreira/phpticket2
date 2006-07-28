<?php 
/**************************************   
*** File: index.php 
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

	require_once('conf.inc');
	require_once('functions01.inc'); // Not sure if it's needed.
	mysql_open();// Not sure if it's needed.
	global $debug;	// Not sure if it's needed.

$this_is =get_variable('version');
if ( strstr( $this_is,"devel" ) )
{
	$this_title = $this_title . "-DEVEL-";
}
	
print '
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
	<HEAD>
		<title>'.$this_title.'</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<LINK REL="StyleSheet" HREF="default.css" TYPE="text/css">
	</HEAD>
	<FRAMESET ROWS="50,*" BORDER="1">
		<FRAME SRC="top.php" NAME="top" SCROLLING="no">';
		print '<FRAME SRC="'.$GLOBALS['MAIN_PAGE'].'.php" NAME="main">
		<NOFRAMES>
		<BODY>
			<b>'.$this_title.'</b> requires a frame capable browser.
		</BODY>
		</NOFRAMES>
	</FRAMESET>
</HTML>';
?>
