<?php
/*********************************************************
*** File: functions01.inc  ***************************
*** Project: ticket2 (phpTicket New Generation)   ********
**********************************************************
*** Author: Sinner from the Prairy **********************
*** email: sinnerbofh@gmail.com ************************
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
*********************************************************/



/**************************************************************
*** function check_for_rows($query) ,courtesy of Micah Snyder *
*** Checks if the resulting query has any rows ****************
**************************************************************/
function check_for_rows($query)
{
	global $debug;
	if($debug) print'<p>functions01.inc::check_for_rows(): $query ' .  $query .'<br>';
	if($sql = mysql_query($query))
	{
		if(mysql_num_rows($sql) !== 0)	return mysql_num_rows($sql);
		else		return false;
	}
	else 	return false;
}


/*************************************************************
*** function debug_output() **********************************
*** output useful information for debugging  *****************
*************************************************************/
function debug_output()
{
	// get mysql version
	$result = mysql_query('SHOW VARIABLES LIKE \'version\'');
	$row 	= mysql_fetch_array($result);
	print '<p>MySQL version: <B>'.$row[Value].'</B><BR>';
	print 'PHP version: <B>'.phpversion().'</B><BR>';
	print 'Users in database: <B>'.check_for_rows('SELECT * FROM user').'</B><BR>';
	print 'Weeks in database: <B>'.check_for_rows('SELECT * FROM weeks').'</B><BR>';
	//get all settings
	$result = mysql_query('SELECT * FROM settings');
	print '<B>Settings</B> <ul>';
	while($row = mysql_fetch_array($result))
	{
		print '<li><p> Setting \''.$row[name].'\': <B>'.$row[value].'</B><BR> </li>';
	}
	print '</ul>';
	print '<BR>Send this to <A HREF="mailto:'.get_variable('admin_email').'">'.get_variable('admin').'</A> if needed.<BR>';
	die;
}


/**************************************************************
*** function do_error($err_function,$err,$custom_err = '') ****
*** raise an error event  *************************************
**************************************************************/
function do_error($err_function,$err,$custom_err = '')
{
	print '<FONT CLASS="warn">An error occured in function "'.$err_function.'": "'.$err.'"<BR>';
	if ($custom_err != '') print 'Additional info: "'.$custom_err.'"<BR>';
	print '<BR>Check your MySQL connection and if the problem persist, contact the <A HREF="help.php?q=credits">author</A>.<BR>';
	die('<B>Execution stopped.</B></FONT>');
}


/*************************************************************
*** function do_login($requested_page) ***********************
*** check for a proper login/session code in given webpage ***
*************************************************************/
function do_login($requested_page)
{
	global $debug;
	
	mysql_open();
	session_start();
	if(get_variable('allow_anonymous_login'))
	{
		$auth = True;
		session_cache_limiter('private_no_expire');
		$cache_limiter = session_cache_limiter();

		// session_cache_expire does not work in older PHP versions!!!
		// set the cache expire to 8 hours ( = 480 minutes) 
		//session_cache_expire(480);
		//$cache_expire = session_cache_expire();

		/* start the session */
		session_start();
		session_register('auth');
		$_SESSION['ticket_username']      = 'anonymous';
		$_SESSION['ticket_user_is_admin'] = 2; // Make it admin
		return;
	}
	if(!session_is_registered('auth'))
	{
		// Name of table of users: user/users/...
		$userdb='user';
		if(check_for_rows("SELECT `user`,`passwd` FROM `$userdb` WHERE `user` = '$_POST[frm_user]' AND `passwd` = PASSWORD('$_POST[frm_passwd]')"))
		{
			$auth = True;
			session_cache_limiter('private_no_expire');
			$cache_limiter = session_cache_limiter();
	
			// session_cache_expire does not work in older PHP versions!!!
			// set the cache expire to 8 hours ( = 480 minutes) 
			//session_cache_expire(480);
			//$cache_expire = session_cache_expire();
			
			/* start the session */
			session_start();
			session_register('auth');

			$query = "SELECT `user`,`admin` FROM `$userdb` WHERE `user` = '$_POST[frm_user]'";
			$result = mysql_query($query) or
						do_error("functions01.inc::do_login()::mysql_query()", 'mysql query failed', mysql_error());
			$row = mysql_fetch_array($result);

			$_SESSION['ticket_username']      = $_POST['frm_user'];
			$_SESSION['ticket_user_is_admin'] = $row['admin'];
			if ($debug)
			{
				print '<p>functions01.inc::do_login()';
				print '<br>Session username = '.$_SESSION['ticket_username'];
				print '<br>Form User = '.$_POST['frm_user'];
				print '<br>Session user is admin = '.$_SESSION['ticket_user_is_admin'];
				print '<br>Print admin = '.$row['admin'];
			}
			return;
		}
		else
		{
			print '<HTML> <HEAD><LINK REL=StyleSheet HREF="default.css" TYPE="text/css">	</HEAD>'."\n".'<CENTER><BR>';
			if (get_variable('version') != '') print get_variable('login_banner').get_variable('admin_department');
			print ', '.get_variable('organization')."</FONT>\n";
			print '<BR><BR><FORM METHOD="post" ACTION="'. $requested_page.'">';
			print "\n<TABLE BORDER=\"0\">";
			if ($_POST['frm_user'] != '') print '<TR><TD COLSPAN="2"><FONT CLASS="warn">Login failed. Try again.</FONT></TD></TR>';
			print '<TR><TD CLASS="td_label">User:</TD>
						<TD><INPUT TYPE="text" NAME="frm_user" MAXLENGTH="255" SIZE="30"></TD></TR>'."\n";
			print '<TR><TD CLASS="td_label">Password: &nbsp;&nbsp;</TD>
						<TD><INPUT TYPE="password" NAME="frm_passwd" MAXLENGTH="255" SIZE="30"></TD></TR>'."\n";
			print '<TR><TD></TD><TD><INPUT TYPE="submit" VALUE="Log In"></TD>';
			print "</TR>\n	</FORM></CENTER> </HTML>";
			exit();
		}
	}
}


/************************************************************
*** function do_logout() ************************************
*** Destroys the current session ****************************
************************************************************/
function do_logout()
{
	session_start();
	session_unset();
	session_destroy();
}


/**************************************************************
*** function do_title($text="Your Company, Inc.") *****
*** Build a title for a webpage *******************************
**************************************************************/
function do_title($text="Your Company, Inc.",$use_headers=1)
{
	global $debug;
	if($debug)
	{
		print '<p> functions01.inc::do_title()';
		print '<br>$text = '.$text;
		print '<br>$use_headers = '.$use_headers;
	}
	if ($use_headers) { set_htmlheader($text); }
	print '</head> 	<body>';
	print '<h2 class="td_black" align="center">'.$text.'</h2>';
	print "<!-- This has added the html headers & title -->\n";
	print '<!-- Session http referer is #'. 	$_SESSION['HTTP_REFERER'] ."# \n ";
	print '<br>basename server php self is #' . basename($_SERVER['PHP_SELF'])."# --> \n ";
	//$_SESSION['HTTP_REFERER'] = basename($_SERVER['PHP_SELF']);
}


/**************************************************************
*** function edit_options($action) ****************************
*** Manage Input options for the different tables *************
**************************************************************/
function edit_options($action)
{
	// This function will do "pre-sorting" of available activities for Input Options
	// See other "function  edit_options_*() "
	global $debug;
	$url = 'config.php?mode=optionstable';
	// The following Array variables hold values, and all are related by index number.
	// So values in different arrays that have commmon index number are a group
	// $current holds: option type,
	// $current_desc holds: option description
	// $current_table holds: option table name	$label_sort= array("ID", "Sort Order", "Value");
	//$label_name = array("ID", "Field Order", "Short Description", "Long Description"); // "Description"); //
	$current = array("sort","name") ; //,"fieldname");
	$current_desc = array("Field Sorting and Visibility","Field Descriptions"); //, "Field Names");
	$current_table = $GLOBALS['MAIN_TABLE'];
	//$current_select = array ("sort" => " AND (`field` LIKE \"short\" OR `field` LIKE \"long\") " , "name" => "blah");

	//$elem = 0;
	$var = 0;

	if ($debug)
	{
		print '<p>functions01.inc::edit_options()';
		print '<br>$ation ="'. $action .'"';
		print '<br>$_GET["go"] = "'.$_GET["go"].'"';
		print '<br>$_GET["current"] = "'.$_GET["current"] .'"';
		print '<br>$_GET["table"] = "'.$_GET["table"] .'"';
	}

	switch ($action)
	{
	case "nogo":
		print '<div id="navvert"><h3>&nbsp; Input Options</h3>';
		print '<ul>';
		// Display available options.
		while ($current[$var])
		{
			$url_add = "&go=edit&current=".$current[$var] . "&table=".$current_table;
			print '<li><a href="'.$url .$url_add.'">'.$current_desc[$var].'</a></li>'  . " \n ";
			$var++;
		}
		print "</ul></div> \n ";
	break;
	case "edit":
	case "update":
		switch ($_GET["current"])
		{
			case "sort":
				edit_options_sort($action);
			break;
			case "name":
				edit_options_description($action);
			break;
		}
	} // End of    switch ($action)
	return;
}


/**************************************************************
*** function edit_options_description($action) ****************
*** Edit Input options: Field description Management***********
**************************************************************/
function edit_options_description($action)
{
	// The goal is to manage main database "trans2" field descriptions, both long and short descriptions
	global $debug;
	$url = 'config.php?mode=optionstable';
	// The following Array variables hold values, and all are related by index number.
	// So values in different arrays that have commmon index number are a group
	// $current holds: option type,
	// $current_desc holds: option description
	// $current_table holds: option table name	$label_sort= array("ID", "Sort Order", "Value");
	$label_name = array("ID", "Field Name", "Short Description", "Long Description"); // "Description"); //
	//$current = array("sort","name","fieldname");
	//$current_desc = array("Sorting and Visibility","Description", "Field Names");
	$current_table = $GLOBALS['MAIN_TABLE'];
	$current_select =  " AND (`field` LIKE \"short\" OR `field` LIKE \"long\") " ;
	$current_title = "Field Descriptions";
	//$query_end ="";
	$elem = 0; $var = 0;

	if ($debug)
	{
		print '<p>functions01.inc::edit_options_description()';
		print '<br>$ation ="'. $action .'"';
		print '<br>$_GET["go"] = "'.$_GET["go"].'"';
		print '<br>$_GET["current"] = "'.$_GET["current"] .'"';
		print '<br>$_GET["table"] = "'.$_GET["table"] .'"';
	}

	switch ($action)
	{
	case "edit":
		// Retrieve fieldnames on $current_table
		$field_query = "SHOW COLUMNS FROM `$current_table` ";
		if ($debug)
		{
			print '<br> $field_query = "'. $field_query .'"';
		}
		$field_result = mysql_query($field_query) or
			do_error('functions01.inc::edit_options_description()::mysql_query()', 'mysql query failed ="'.$field_query.'"', mysql_error());
		$table_fields = mysql_fetch_array($field_result);
		// Retrieve existing descriptions. Query goes something like this:
		// 	SELECT `id`, `field`, `value` FROM `options`
		// 		WHERE 1 AND `table_data` LIKE 'trans2'
		// 		AND (`field` LIKE "short" OR `field` LIKE "long")
		// 		ORDER BY `id` ASC LIMIT 100
		$query = "SELECT `id`, `value` FROM `options`";
		$query .= " WHERE 1 AND `table_data` LIKE '".$current_table."' ";
		$query .=  $current_select . " ORDER BY `id` ASC ";
		if ($debug)  // (1) //
		{
			print '<br> $query = "'. $query .'"';
		}
		$result = mysql_query($query) or
			do_error('functions01.inc::edit_options_description()::mysql_query()', 'mysql query failed ="'.$query.'"', mysql_error());
		$row = mysql_fetch_array($result);
		// Error message if there are no results found.
		if (!$row[0]) {  print '<p class="warn">There are no elements to show!" (function01::edit_options_description)</p>';  return 1;}
		//$maxrows = mysql_num_rows($result);
		// As we use this fetch_array, we will use do{} instead of while{}
		print '<table><tr><td>' ." \n ";
		print '<TABLE BORDER="0" CELLPADDING="2">'. " \n ";
		//First, the headers. Header COLSPAN depends on the number of retrieved fields, plus one field for the button
		print '<TR><TD class="thethead" COLSPAN='. (sizeof($label_name) + 1 ).'>' . $current_title;
		print '</TD></TR> '." \n ";
		print '<TR>';
		$num = 0;
		while ($label_name[$num])
		{
			print '<TD  CLASS="td_label">'.$label_name[$num].'</TD>' . " \n ";
			$num++;
		}
		print '<TD  CLASS="td_label">Send Changes</TD>';
		print "</TR> \n";
		// Now, the editable values
		$tab = 0;
		do {
			print '<TR>';
			print '<TD valign="top"><FORM METHOD="post" ACTION="'.$url.'&go=update">' . " \n ";
			for ($num = 0; $num < 2 ; $num++)  // $num=0 is "id", as 'id' is an **auto** field, skip it & print rest of fields
			{
				switch($num){
				case (0):
					print '<INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$row[0].'">' . " \n ";
					print '<B>'.$row[0].'</B>';// <FORM METHOD="post" ACTION="'.$url.'go=update">';
					print '<INPUT TYPE="hidden" NAME="frm_table" VALUE="'.$_GET["table"].'">' . " \n ";
					print '<INPUT TYPE="hidden" NAME="frm_field" VALUE="'.$_GET["field"].'">' . " \n ";
					print "</TD> \n";
					print '<TD class="tr_user" >';
					print '<INPUT TYPE="hidden" NAME="frm_field" VALUE="'.$table_fields['Field'].'">'.$table_fields['Field'] . " \n ";
					$table_fields = mysql_fetch_array($field_result);
				break;
				case (1):
					print '<TD  >';
					print '<INPUT TYPE="text" SIZE="15" MAXLENGTH="11" tabindex="'.$tab.'" NAME="frm_short" VALUE="'.$row[$num].'">';
					print '</TD><TD  >';
					$row = mysql_fetch_array($result);
					print '<INPUT TYPE="text" SIZE="30" MAXLENGTH="30" tabindex="'.($tab++).'" NAME="frm_long" VALUE="'.$row[$num].'">' . " \n ";
				break;
				} // End switch ($num)
				print "</TD> \n";
				$tab++;
			} // End for ($num...)
			print '<TD COLSPAN="3" align="center"><INPUT TYPE="submit" VALUE="Submit Changes"></FORM></TD>';
			print "</TR> \n";
		} while ($row = mysql_fetch_array($result)) ;
		// End of table. Now, print some help.
		print "</table> \n ";
		print '</TD>';
		print '<TD valign="top">'; // class="severity_medium">';
		print '<div id="help">Rules and guidelines:<ol>';
		print '<li>DO NOT DELETE any existing value</li>';
		print '<li>To "eliminate" any existing value from the available options, set "Sort Order" to Zero 	</li>';
		print '<li>After modifying one entry, click on its "Submit Changes" button</li>';
		print '<li>To add a new entry, type its value in the bottom blank entry, then click on "Add New"</li>';
		print '<li>Make sure there are no repeated numbers in the column Sort Order</li>';
		print '<li>To make one value the "Default Value", give it  the number "1" on Sort Order"</li>';
		print '<li>To make no value the "Default Value", make sure that a value "--" exists and that it has  the number "1" on Sort Order"</li>';
		print '<li></li>';
		print '</ol></div></TD>';
		print '</tr><table>';
	break;
	case "update":
		global $link_next ;
		global $mysql_db_next;
		//if (!$_GET["frm_0"]) //
		if ($_POST[frm_0] == "")
		{
			$startquery = "INSERT INTO `".$GLOBALS['SELECT_TABLE']."` (`id` , `orden` , `table_data` , `field` , `value`) ";
			$endquery   = " VALUES ('', '$_POST[frm_1]', '$_POST[frm_table]', '$_POST[frm_field]', '$_POST[frm_2]')";
			$done = "New Option has been added";
		} else {
			$startquery = "UPDATE `".$GLOBALS['SELECT_TABLE']."` SET ";
			$endquery   = " `orden` = '$_POST[frm_1]', `table_data` = '$_POST[frm_current]', `field` = '$_POST[frm_field]', ";
			$endquery   = $endquery . " `value` = '$_POST[frm_2]' WHERE `id` = '$_POST[frm_0]' LIMIT 1 ";
			$done = "Option has been modified";
		}
		if  ($debug)
		{
			print '<br>$query is "'. $startquery . $endquery .'"<br>';
		}
		//mysql_open();
		$result = mysql_query($startquery . $endquery)
			or do_error('functions01.inc::edit_options_description:update', 'mysql_query() failed', mysql_error);
		print '<p class="severity_medium">' . $done;
	break;
	} // End of    switch ($action)
	return;
}


/**************************************************************
*** function edit_sys_options($action) ****************************
*** Edit Input options for the different tables ***************
**************************************************************/
function edit_input_options($action)
{
	global $debug;
	$url = 'config.php?mode=optionstable';
	$labels= array("ID", "Sort Order", "Field",  "Value");
	$current = array("ticket", "location", "action", "status", "action_description"); //"severity",
	$ticket = array("descriptions","visibility");

	$elem=0; $var=0;
	if ($debug)
	{
		print '<p>functions01.inc::edit_input_options()';
		print '<br>$ation ="'. $action .'"';
		print '<br>$_GET["go"] = "'.$_GET["go"].'"';
		print '<br>$_GET["table"] = "'.$_GET["table"] .'"';
		print '<br>$_GET["field"] = "'.$_GET["field"] .'"';
	}
//	print '<p class="severity_medium">Below is the list of dynamic fields. Click on one link to edit or add new options for that option</p>';

//	print '<table cellpadding="5">';
//	print '<tr>';
	switch ($action)
	{
	case "nogo":
		print '<h3 align="center"  class="thead">Edit Input Options</h3>';
		print '<p class="severity_medium">Below is the list of dynamic fields. Click on one link to edit or add new options for that option</p>';
		print '<table cellpadding="5">' . " \n ";
		print '<tr>';
		while ($current[$var])
		{
			$url_add = "&go=edit&table=".$current[$var];
			print '<td valign="top">';
			print '<table cellpadding="4">' . " \n ";
			print '<thead ><tr >';
			print '<td class="td_header">Options for "'.$current[$var].'"</td>';
			print '</tr></thead>' . " \n ";
			print '<tbody>';
			switch ($current[$var])
			{
			case ("ticket"):
				while (${$current[$var]}[$elem])
				{
					$url_add_on = "&field=".${$current[$var]}[$elem];
					print '<tr><td><a href="'.$url .$url_add. $url_add_on.'">Edit '.${$current[$var]}[$elem].'</a></td></tr>' . " \n ";
					$elem++;
				}
			break;
			case ("location"):
			$url_add_on = "&field=".($current[$var]);
				print '<tr><td><a href="'.$url .$url_add. $url_add_on.'"> Edit '.($current[$var]).'s</a></td></tr>'  . " \n ";
				$elem++;
			break;
			}
			print "</tbody> \n ";
			print "</table> \n ";
			print '</td>';
			$var++;
		}
		print "</tr></table> \n ";
	break;
	case "edit":
		$query = "SELECT `id`, `orden`, `table_data`, `field`, `value` FROM `".$GLOBALS['SELECT_TABLE'] ."`";
		$query = $query ." WHERE `table_data` LIKE '".$_GET["table"]."' "; //AND `field` LIKE '".$_GET["field"]."' ";
		if ($debug)
		{
			print '<br>functions01.inc::edit_sys_options().';
			print '<br>$query = "'. $query ."\" \n ";
		}
		$result = mysql_query($query) or
			do_error('functions01.inc::edit_input_options()::mysql_query()', 'mysql query failed ="'.$query.'"', mysql_error());
		$row = mysql_fetch_array($result);
		if (!$row[0]) {  print '<p class="warn">There are no elements to show!"</p>';  return 1;}
		$maxrows=mysql_num_rows($result);
		// As we use this fetch_array, we will use do{} instead of while{}
		print '<table><tr><td>' . " \n ";
		print '<TABLE BORDER="0" bgcolor="white" CELLPADDING="5"><THEAD>'. " \n ";
		// table "options" has "5" fields.
		//First, the headers.
		print '<TR><TD class="thead" align="center" COLSPAN=5>'. $_GET["table"] .'. Option: "'.$_GET["field"]. '"</TD>'."</TR>  \n ";
		print '<TR>';
		$num=0;
		while ($labels[$num])
		{
			print '<TD  CLASS="td_label" align="center" NOWRAP>'.$labels[$num].'</TD>' . " \n ";
			$num++;
		}
		print '<TD  CLASS="td_label" align="center" NOWRAP>Send Changes</TD>';
		print "</TR></THEAD> \n";
		// Now, the editable values
		$tab=0;
		do {
			print '  <TR>';
			print '<TD valign="top">'." \n " .'<FORM  METHOD="post" ACTION="'.$url.'&go=update">' . " \n ";
			for ($num = 0; $num < 5 ; $num++)  // $num=0 is "id", as 'id' is an **auto** field, skip it & print rest of fields
			{
				switch($num){
				case (0):
					print '<INPUT TYPE="hidden" NAME="frm_'.$num.'" VALUE="'.$row[$num].'">' . " \n ";
					print '<B>'.$row[$num].'</B>';// <FORM METHOD="post" ACTION="'.$url.'go=update">' . " \n ";
					print '<INPUT TYPE="hidden" NAME="frm_table" VALUE="'.$_GET["table"].'">' . " \n ";
					print '<INPUT TYPE="hidden" NAME="frm_field" VALUE="'.$_GET["field"].'">' . " \n ";
					print '<INPUT TYPE="hidden" SIZE="10" NAME="frm_2" VALUE="'.$row[2].'">' . " \n ";
				break;
				case (1):
					print '<TD><INPUT TYPE="text" SIZE="3" MAXLENGTH="2" tabindex="'.$tab.'" NAME="frm_'.$num;
					print '" VALUE="'.$row[$num].'">';
					$num++;
				break;
				case (2):
					//print '<TD><INPUT TYPE="hidden" SIZE="10" NAME="frm_'.$num.'" VALUE="'.$row[$num].'">'.$row[$num];
				break;
				case (3):
				case (4):
					print '<TD><INPUT TYPE="text" SIZE="20" MAXLENGTH="25" tabindex="'.$tab.'" NAME="frm_'.$num;
					print '" VALUE="'.$row[$num].'">';
				break;
				} // End switch ($num)
				print "</TD> \n ";
				$tab++;
			} // End for ($num...)
			print '<TD COLSPAN="3" align="center"><INPUT TYPE="submit" VALUE="Submit Changes"></FORM></TD>'. " \n ";
			print "</TR> \n";
		} while ($row = mysql_fetch_array($result)) ;
		//Now, area for a new entry
		print '  <TR>';
		print '<TD><FORM  METHOD="post" ACTION="'.$url.'&go=update"><INPUT TYPE="hidden" NAME="frm_0" VALUE=""> <B>New!</B>';
		print '<INPUT TYPE="hidden" NAME="frm_table" VALUE="'.$_GET["table"].'">';
		print '<INPUT TYPE="hidden" NAME="frm_field" VALUE="'.$_GET["field"].'">' ."</TD> \n";
		print '<TD> &nbsp; <INPUT TYPE="text" SIZE="3" MAXLENGTH="2" tabindex="'.$tab.'" NAME="frm_1" VALUE="">'."</TD> \n";
		print '<TD><INPUT TYPE="text" SIZE="35" MAXLENGTH="30" tabindex="'.($tab+1).'" NAME="frm_2" VALUE="">'."</TD> \n";
		print '<TD COLSPAN="3" align="center"><INPUT TYPE="submit" VALUE="Add New"></FORM></TD>';
		print "</TR> \n";
		print "</table> \n ";
		print '</TD>';
		print '<TD valign="top" class="severity_medium">Rules and guidelines:<ol>';
		print '<li>DO NOT DELETE any existing value</li>';
		print '<li>To "eliminate" any existing value from the available options, set "Sort Order" to Zero	</li>';
		print '<li>After modifying one entry, click on its "Submit Changes" button</li>';
		print '<li>To add a new entry, type its value in the bottom blank entry, then click on "Add New"</li>';
		print '<li>Make sure there are no repeated numbers in the column Sort Order</li>';
		print '<li>To make one value the "Default Value", give it  the number "1" on Sort Order"</li>';
		print '<li>To make no value the "Default Value", make sure that a value "--" exists and that it has  the number "1" on Sort Order"</li>';
		print '<li></li>';
		print '</ol></TD>';
		print '</tr><table>';

	break;
	case "update":
		global $link_next ;
		global $mysql_db_next;
		//if (!$_GET["frm_0"]) //
		if ($_POST[frm_0] == "")
		{
			$startquery = "INSERT INTO `".$GLOBALS['SELECT_TABLE']."` (`id` , `orden` , `table_data` , `field` , `value`) ";
			$endquery   = " VALUES ('', '$_POST[frm_1]', '$_POST[frm_table]', '$_POST[frm_field]', '$_POST[frm_2]')";
			$done = "New Option has been added";
		} else {
			$startquery = "UPDATE `".$GLOBALS['SELECT_TABLE']."` SET ";
			$endquery   = " `orden` = '$_POST[frm_1]', `table_data` = '$_POST[frm_table]', `field` = '$_POST[frm_field]', ";
			$endquery   = $endquery . " `value` = '$_POST[frm_2]' WHERE `id` = '$_POST[frm_0]' LIMIT 1 ";
			$done = "Option has been modified";
		}
		if  ($debug)
		{
			print '<br>$query is "'. $startquery . $endquery .'"<br>';
		}
		//mysql_open();
		$result = mysql_query($startquery . $endquery)
			or do_error('functions01.inc::edit_input_options:update', 'mysql_query() failed', mysql_error);
		print '<p class="severity_medium">' . $done;
	break;
	} // End of    switch ($action)
	return;
}


/*************************************************************************
*** function index_elements($irows,$limit) *******************************
*** Build an index of pages with results, like google ********************
*** from http://www.webestilo.com/foros/mensaje.phtml?foro=13&msg=9629 ***
*************************************************************************/
function index_elements($irows,$current,$per_page,$order,$SQL="",$isclosed="")
{
	// $irows: total number of data entries to show & piece in viewable pages
	// $current: current element displayed. That is, the first entry/row displayed in the page.
	// $per_page : number of entries per page
	// $order: order SQL sort, either ASC or DESC
	// $SQL: query for searches
	// $isclosed : if we are dealing with CLOSED tickets, we must add "showclosed=true" to any clickeable URL		
	// The link on the page has to be similar to this:
	// print '<TH class="thead"><A HREF="?sortby='. $ident . "&order=".$order .'&search='.$search.'" CLASS="td_link">'.$ident.'</A></TH>';
	global $element;
	global $debug;
	$index_of_pages=""; // Start the string to be returned
	if ($isclosed != "") // If we want to limit tickets to closed, add this to all URLs generated here
	{
		$url_addon = "&showclosed=true";
	}
	if ($debug) // For Debug
	{
		print "<p>functions01.inc::index_elements() pre \$order is ".$order;
		print "<br>functions01.inc::index_elements() pre \$_GET['sortby'] is ".$_GET['sortby'] ;
		print "<br>functions01.inc::index_elements() pre \$isclosed is ".$isclosed ;
		print "<br>functions01.inc::index_elements() pre \$SQL is ".$SQL ."\n";
	}
	// From functions01.inc::list_data()
	$sortby = $_GET['sortby'] == '' ? 'id' : $_GET['sortby'] ;
	if ($debug) // For Debug
	{
		print "<p>functions01.inc::index_elements() post \$order is $order";
		print "<br>functions01.inc::index_elements() post \$sortby is $sortby";
		print "<br>functions01.inc::index_elements() pre \$isclosed is ".$isclosed ;
		print "<br>functions01.inc::index_elements() post \$SQL is ".$SQL ." \n";
	}
	$total_pags = ($irows/$per_page);	// Total Number of Pages
	$numpag = 0; // Current Page number shown on Pages list
	$current_pag=($current/$per_page); // (Current page *minus one*) shown in BOLD RED. So Page "one" makes this "zero".
	$i = 0; // Initialize counter of elements, also known as "rows" or "lines"
	if ($debug)
	{
		print "<p>functions01.inc::index_elements() Total Num of Pages is " . ($total_pags) ;
		print "<br>functions01.inc::index_elements() \$numpag is $numpag";
		print "<br>functions01.inc::index_elements() \$current_pag = $current_pag";
		print "<br>functions01.inc::index_elements() \$i = $i \n";
	}
	// Index of elements shown
	print "<TABLE BORDER='0' ALIGN='center' WIDTH='100%'><TR>";
	$index_of_pages="<TABLE BORDER='0' ALIGN='center' WIDTH='100%'><TR>";
	print "<TD ALIGN='left'>";
	$index_of_pages = $index_of_pages."<TD ALIGN='left'>";
	// If we have many pages, use the code. If not, go simple.
	if ($total_pags >10)
	{
		for ($i; $i< $irows; $i=$i+$per_page)
		{
			$numpag = $numpag + 1; //Advance page number
			switch (TRUE)
			{
			case (($current_pag + 1) < 6 ): // We start with numbers, but we end with [...]. $current_pag starts with 0 --> $c_p +1
				//if ($debug)  print "<p>\$current is $current ||\$i is $i ||\$current_pag is $current_pag +1 (lower than 6)";
				// Print current page number with link to "proper limited search" ( = 'page')
				if ($numpag > 9)
				{	// After page [9] , we get to page [..], which points to page [10]
					$numpag="..";
					print '<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">['.$numpag.']</A> ';
					$index_of_pages = $index_of_pages.'<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby
								.'&SQL='.$SQL.'">['.$numpag.']</A> ';
					$i = $irows;
				} else {
					if ($current != $i)
					{
						print '<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">['.$numpag.']</A> ';
						$index_of_pages = $index_of_pages.'<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby
									.'&SQL='.$SQL.'">['.$numpag.']</A> ';
					} else { // ..else... Print in bold-red current page, with no link
						print "<font color=\"red\" face=\"bolder\" >[".$numpag."]</font> ";
						$index_of_pages = $index_of_pages."<font color=\"red\" face=\"bolder\" >[".$numpag."]</font> ";
					}
				}
			break;
			case (($current_pag + 1 ) > 5 ): // We start with [...] . $current_pag starts with 0 --> $c_p +1
				//if ($debug)  print "<p>\$current is $current ||\$i is $i ||\$current_pag is $current_pag +1 (greater than 5)";
				// First one will be a [..], and points to page (current_page -5)
				if ($i == 0)
				{
					// If we are in the first page...
					//if ($debug) print "<p>\$i is zero: $i<br>";
					$i = (($current_pag + 1 ) - 6 ) * $per_page ; // Not sure about the +1 in there. Yes, +1 it's *good*.
					$numpag =  ( ( ($current_pag + 1)  - 6 ) + 1  );  // Final '+1' in expresion, makes advance one page, as needed.
					print '<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">[..]</A> ';
					$index_of_pages = $index_of_pages.'<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">[..]</A> ';
					//if ($debug)  print "<p>As \$i = zero, \$i = $i || and \$numpag = $numpag <br>";
				} else {
					if ( $numpag < ($current_pag + 1 ) + 5 )  // If we are within the 9 pages around the current page, all OK
					{
						// Print current page number with link to "proper limited search" ( = 'page')
						//if ($debug) print "<p>\$i is not zero: $i<br>";
						if ($current != $i)
						{
							print '<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">['.$numpag.']</A> ';
							$index_of_pages = $index_of_pages.'<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby
										.'&SQL='.$SQL.'">['.$numpag.']</A> ';
						} else { // ..else... Print in bold-red current page, with no link
							print "<font color=\"red\" face=\"bolder\" >[".$numpag."]</font> ";
							$index_of_pages = $index_of_pages."<font color=\"red\" face=\"bolder\" >[".$numpag."]</font> ";
						}
					} else { // If we are beyond (current-page + 4) , page-number-to-print is too big
						// If page-number-to-print is too big, we print [..] instead and we exit the loop.
						$numpag="..";
						print '<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">['.$numpag.']</A> ';
						$index_of_pages = $index_of_pages.'<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby
									.'&SQL='.$SQL.'">['.$numpag.']</A> ';
						$i = $irows;
					}
				}
			break;
			default:
				print "<p>Nothing to show just yet, as \$total_pags equals $total_pags<br>\n";
				$i = $irows; // Terminate for(...) fast!
			break;
			}
		}
	} else { // $total_pags < 11  , we go simple
		//if ($debug) print "<p> \$total_pags is <9 = $total_pags";
		for ($i; $i < $irows; $i = $i + $per_page)
		{
			$numpag = $numpag+1; //Advance page number
			// Print current page number with link to "proper limited search" ( = 'page')
			if ($current != $i)
			{
				print '<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby.'&SQL='.$SQL.'">['.$numpag.']</A> ';
				$index_of_pages = $index_of_pages.'<A HREF="?current='.$i.$url_addon.'&order='.$order.'&sortby='.$sortby
							.'&SQL='.$SQL.'">['.$numpag.']</A> ';
			} else { // ..else... Print in bold-red current page, with no link
				print "<font color=\"red\" face=\"bolder\" >[".$numpag."]</font> ";
				$index_of_pages = $index_of_pages."<font color=\"red\" face=\"bolder\" >[".$numpag."]</font> ";
			}
		}
	}
	print "</TD></TR></TABLE>\n";
	$index_of_pages = $index_of_pages."</TD></TR></TABLE>\n";
	return $index_of_pages;
}


/**************************************************************
*** function edit_options_sort($action) ***********************
*** Edit Input options: Field sorting *************************
**************************************************************/
function edit_options_sort($action)
{
	global $debug;
	$url = 'config.php?mode=optionstable';
	// The following Array variables hold values, and all are related by index number.
	// So values in different arrays that have commmon index number are a group
	// $current holds: option type,
	// $current_desc holds: option description
	// $current_table holds: option table name	$label_sort= array("ID", "Sort Order", "Value");
	//$label_name = array("ID", "Description", "Field Order"); // "Description"); //
	$label_name = array ("Desciption", "Field Order &nbsp; &nbsp; ");
	$current = array("sort","name","fieldname");
	$current_desc = array("Sorting and Visibility","Description", "Field Names");
	$current_table = $GLOBALS['MAIN_TABLE'];
	$current_fields = array ("id", "value", "orden");
	$current_select = " AND `field` LIKE \"long\" ";
	$query_end = "";
	$elem = 0; $var = 0;

	if ($debug)
	{
		print '<p>functions01.inc::edit_options_sort()';
		print '<br>$ation ="'. $action .'"';
		print '<br>$_GET["go"] = "'.$_GET["go"].'"';
		print '<br>$_GET["current"] = "'.$_GET["current"] .'"';
		print '<br>$_GET["table"] = "'.$_GET["table"] .'"';
	}

	switch ($action)
	{
	case "nogo":
		print '<div id="navvert"> <h3 >Fields Manager Choices</h3>';
		print '<ul>';
		// Display available options.

		while ($current[$var])
		{
			$url_add = "&go=edit&current=".$current[$var] . "&table=".$current_table;
			print '<li><a href="'.$url .$url_add.'">'.$current_desc[$var].'</a></li>'  . " \n ";
			$var++;
		}
		print "</ul></div> \n ";
	break;
	case "edit":
	// Build the query to show all descriptions.
	//SELECT `id`, `orden`, `field`, `value` FROM `options` WHERE 1 AND `table_data` LIKE 'trans2' AND (`field` LIKE "short" OR `field` LIKE "long") ORDER BY `id` ASC LIMIT 100
		//$query = "SELECT `id`, `value`, `orden`  FROM `options`";
		$query = "SELECT ";
		// get all elements from $current_fields array
		foreach($current_fields as $item) {
			$query .= " `". $item ."`,";
		}
		//remove_trailing_comma($query);
		$query = rtrim($query, ",");
		if ($debug)
		{
			print '<br> Trimmed partial $query is = "'. $query .'"';
		}
		$query .= " FROM `options` WHERE 1 AND `table_data` LIKE '".$_GET["table"]."' ";
		$query .= $current_select . " ORDER BY `orden` ASC ";
		if ($debug)
		{
			print '<br> $query = "'. $query .'"';
		}
		$result = mysql_query($query) or
			do_error('functions01.inc::edit_options_sort()::mysql_query()', 'mysql query failed ="'.$query.'"', mysql_error());
		$row = mysql_fetch_array($result);
		// Error message if there are no results found.
		// Commenting out next line, as iy might use up one $row[]  array
		//if (!$row[0]) {  print '<p class="warn">There are no elements to show!"</p>';  return 1;}
		//$maxrows = mysql_num_rows($result);
		print '<table><tr><td>' ." \n ";
		print '<TABLE BORDER="0" CELLPADDING="2">'. " \n ";
		//First, the headers. Header COLSPAN depends on the number of available labels, plus one field for the button
		print '<TR><TD class="thethead" COLSPAN='. (sizeof($label_name) + 1 ).'>';
		print 'Manage the display order of existing fields';
		print '</TD></TR> '." \n ";
		print '<TR>';
		$num = 0;
		while ($label_name[$num])
		{
			print '<TD  CLASS="td_label" NOWRAP>'.$label_name[$num].'</TD>' . " \n ";
			$num++;
		}
		print '<TD  CLASS="td_label">Send Changes</TD>';
		print "</TR> \n";
		// Now, the editable values
		$tab=0;
		// As we use fetch_array, we will use do{} instead of while{}
		/*******************************************************/
		do {
			print '<TR>';
			print '<TD valign="top"><FORM METHOD="post" ACTION="'.$url.'&go=update">' . " \n ";
			for ($num = 0; $num < 3 ; $num++)  // $num=0 is "id", as 'id' is an **auto** field, skip it & print rest of fields
			{
				switch($num){
				case (0):
					print '<INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$row[$current_fields[$num]].'">' . " \n ";
					//print '<B>'.$row[$num].'</B>';// <FORM METHOD="post" ACTION="'.$url.'go=update">';
					print '<INPUT TYPE="hidden" NAME="frm_table" VALUE="'.$_GET["table"].'">' . " \n ";
					//print '<INPUT TYPE="hidden" NAME="frm_field" VALUE="'.$_GET["field"].'">' . " \n ";
					//$row = mysql_fetch_array($result); $num++;
					$num++;
					// now, case (1):
					print '<INPUT TYPE="hidden" NAME="frm_longname" VALUE="'.$row[$current_fields[$num]].'">'.$row[$current_fields[$num]] ;
				break;
				case (2):
					print '<TD>';
					print '<INPUT TYPE="text" SIZE="3" MAXLENGTH="2" tabindex="'.$tab.'" NAME="frm_order" VALUE="'.$row[$current_fields[$num]].'">' ;
				break;
				//case (1):
				//	print '<TD  >';
				//	print '<INPUT TYPE="hidden" NAME="frm_long" VALUE="'.$row[$num].'">'.$row[$num] . " \n ";
				//break;
				} // End switch ($num)
				print "</TD> \n";
				$tab++;
			} // End for ($num...)
			print '<TD COLSPAN="3" align="center"><INPUT TYPE="submit" VALUE="Submit Changes"></FORM></TD>';
			print "</TR> \n";
		} while ($row = mysql_fetch_array($result)) ;
		/*******************************************************/
/********************************
		foreach( $row as $current_row) {
			print '<TR>';
			print '<TD valign="top"><FORM METHOD="post" ACTION="'.$url.'&go=update">' . " \n ";
			print '<INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$row[$current_fields[$num]].'">' . " \n ";
			//print '<B>'.$row[$num].'</B>';// <FORM METHOD="post" ACTION="'.$url.'go=update">';
			print '<INPUT TYPE="hidden" NAME="frm_table" VALUE="'.$_GET["table"].'">' . " \n ";
			//print '<INPUT TYPE="hidden" NAME="frm_field" VALUE="'.$_GET["field"].'">' . " \n ";
			//$row = mysql_fetch_array($result); $num++;
			// now, case (1):
			print '<INPUT TYPE="hidden" NAME="frm_longname" VALUE="'.$row[$current_fields[$num]].'">'.$row[$current_fields[$num]] ;
			print "</TD> \n";
			$tab++;
			print '<TD valign="top"><FORM METHOD="post" ACTION="'.$url.'&go=update">' . " \n ";
			print '<INPUT TYPE="text" SIZE="3" MAXLENGTH="2" tabindex="'.$tab.'" NAME="frm_order" VALUE="'.$row[$current_fields[$num]].'">' ;
			print '<TD COLSPAN="3" align="center"><INPUT TYPE="submit" VALUE="Submit Changes"></FORM></TD>';
			print "</TR> \n";
		}
********************************/
		print "</table> \n ";
		print '</TD>';
		print '<TD valign="top">'; // class="severity_medium">';
		print '<div id="help">Rules and guidelines:<ol>';
		print '<li>DO NOT DELETE any existing value</li>';
		print '<li>To "eliminate" any existing value from the available options, set "Sort Order" to Zero	</li>';
		print '<li>After modifying one entry, click on its "Submit Changes" button</li>';
		print '<li>To add a new entry, type its value in the bottom blank entry, then click on "Add New"</li>';
		print '<li>Make sure there are no repeated numbers in the column Sort Order</li>';
		print '<li>To make one value the "Default Value", give it  the number "1" on Sort Order"</li>';
		print '<li>To make no value the "Default Value", make sure that a value "--" exists and that it has  the number "1" on Sort Order"</li>';
		print '<li></li>';
		print '</ol></div></TD>';
		print '</tr><table>';

	break;
	case "update":
		global $link_next ;
		global $mysql_db_next;

/***********************************
	case "update":
		global $link_next ;
		global $mysql_db_next;
		//if (!$_GET["frm_0"]) //
		if ($_POST[frm_0] == "")
		{
			$startquery = "INSERT INTO `".$GLOBALS['SELECT_TABLE']."` (`id` , `orden` , `table_data` , `field` , `value`) ";
			$endquery   = " VALUES ('', '$_POST[frm_1]', '$_POST[frm_table]', '$_POST[frm_field]', '$_POST[frm_2]')";
			$done = "New Option has been added";
		} else {
			$startquery = "UPDATE `".$GLOBALS['SELECT_TABLE']."` SET ";
			$endquery   = " `orden` = '$_POST[frm_1]', `table_data` = '$_POST[frm_current]', `field` = '$_POST[frm_field]', ";
			$endquery   = $endquery . " `value` = '$_POST[frm_2]' WHERE `id` = '$_POST[frm_0]' LIMIT 1 ";
			$done = "Option has been modified";
		}
		if  ($debug)
		{
			print '<br>$query is "'. $startquery . $endquery .'"<br>';
		}
		//mysql_open();
		$result = mysql_query($startquery . $endquery)
			or do_error('functions01.inc::edit_options:update', 'mysql_query() failed', mysql_error);
		print '<p class="severity_medium">' . $done;
	break;
***********************************/
	break;
	} // End of    switch ($action)
	return;
}


/***********************************************************************
*** function generate_date_dropdown() **********************************
*** print out date and time in dropdown menus  *************************
***********************************************************************/
function generate_date_dropdown($date_suffix, $default_date=0, $default_start_year=2000, $tab_value=0)
{
	global $debug;
	$value = "";

	if($debug)
	{
			print '<p>functions.inc.php::generate_date_dropdown():';
			print '<br> $date_suffix -> ' . $date_suffix;
			print '<br> $default_date -> ' . $default_date;
			print '<br> $tab_value -> ' . $tab_value;
	}
	//default to current date/time if no values are given
	if ($default_date)
	{
		$year  	= date('Y',$default_date);
		$month 	= date('m',$default_date);
		$day   	= date('d',$default_date);
		$hour  	= date('H',$default_date);
		$minute	= date('i',$default_date);
		//$am_pm 	= date('A',$default_date);
	}else {
		$year 	= date('Y');
		$month 	= date('m');
		$day 	= date('d');
		$hour 	= date('H');
		$minute	= date('i');
		//$ap_pm	= date('A');
	}

	if ($tab_value) {
		$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
	}
	print '<div style="display: inline; vertical-align: bottom;">';	
	print '<SELECT '.$value .' name="frm_year_'.$date_suffix.'">';
	for($i = $default_start_year; $i < 2011; $i++)
	{
		print '<OPTION VALUE="'.$i.'"';
		$year == $i ? (print ' SELECTED>'.$i.'</OPTION>') : (print '>'.$i.'</OPTION>');
	}

	if ($tab_value) {
		$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
	}
	print '</SELECT>&nbsp;'."\n";
	print '<SELECT '.$value .'name="frm_month_'.$date_suffix.'">';
	for($i = 1; $i < 13; $i++)
	{
		print '<OPTION VALUE="'.$i.'"';
		$month == $i ? (print ' SELECTED>'.$i.'</OPTION>') : (print '>'.$i.'</OPTION>');
	}

	if ($tab_value) {
		$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
	}
	print '</SELECT>&nbsp;'."\n";
	print '<SELECT '.$value .'name="frm_day_'.$date_suffix.'">';
	for($i = 1; $i < 32; $i++)
	{
		print '<OPTION VALUE="'.$i.'"';
		$day == $i ? (print ' SELECTED>'.$i.'</OPTION>') : (print '>'.$i.'</OPTION>');
	}
	print '</SELECT>&nbsp;&nbsp;'."\n";
	print '</div>';
	print '<div style="display: inline; ">';
	print "<!-- default:$default_date,$year-$month-$day $hour:$minute -->";
	//print "<!-- default:$default_date,$year-$month-$day  -->";
	print '<INPUT style="margin-bottom: 1em " TYPE="text" SIZE="2" MAXLENGTH="2" NAME="frm_hour_'.$date_suffix.'" VALUE="'.$hour.'">';
	print '<INPUT style="margin-bottom: 1em " TYPE="text" SIZE="2" MAXLENGTH="2" NAME="frm_minute_'.$date_suffix.'" VALUE="'.$minute.'">';
	//print '<INPUT TYPE="text" SIZE="2" MAXLENGTH="2" NAME="frm_ampm_'.$date_suffix.'" VALUE="'.$am_pm.'">';
	print '</div>';
	
	if ($tab_value) return $tab_value;
}


/***********************************************************************
*** function get_fieldname($length,$table_name) ************************
*** Returns aray with table's fieldnames in Human Readable Form, *******
*** in either short or long format *************************************
***********************************************************************/
function get_fieldname($length,$table_name)
{
	global $mysql_user_next;
	global $mysql_passwd_next;
	global $mysql_host_next;
	global $mysql_db_next;
	global $debug;
	$length = ($length == 0 ? "short" : "long");
	// Open Options Table, using $db_***_next values
	mysql_open_next("$mysql_db_next", "$mysql_user_next", "$mysql_passwd_next");
	// Establishing the query
	$query_temp = "SELECT `value` FROM `options` WHERE 1 AND `table_data` LIKE '$table_name' AND `field` LIKE '$length' ";
	$query_temp = $query_temp . "ORDER BY `orden` ASC  LIMIT 0 , 100";
	if ($debug)
	{
			print '<p>functions01.inc::get_fieldname(table_name='.$table_name.')';
			print '::mysql_query()::result_temp<br>query_temp = ' . $query_temp;
	}

	// Retrieving the data.
	$result_temp = mysql_query($query_temp) or
		do_error("functions01.inc::get_fieldname(table_name=$table_name)::mysql_query()::result_temp", 'mysql query failed with query_temp ' . $query_temp, mysql_error());
	//Initialize empty array
	$array_temp = array();
	//Put results from query in an array.
	while ($row_temp = mysql_fetch_array($result_temp)) array_push($array_temp, $row_temp[0]);
	//Return Array
	return $array_temp;
}

/*************************************************************
*** function get_id_from_user($username) *********************
*** get 'id from $user ***************************************
*************************************************************/
function get_id_from_user($username="")
{
	global $debug;
	if ($debug) {
		print '<p>functions01.inc:: get_id_from_user("'.$username.'")';
	}
	
	//if ($username) {
		$query = "SELECT `id`,`user` FROM `user` WHERE `user`='$username'";
		$result	= mysql_query($query) or
			do_error("<p>functions01.inc::get_id_from_user(user:'$username')::mysql_query()", "mysql query failed", mysql_error());
		$row	= mysql_fetch_array($result);
		return $row['id'];
	//} else {
	//	do_error("<p>functions01.inc::get_id_from_user(user:'$username')", "variable '\$username' is void");
	//	die;
	//}
}


/*************************************************************
*** function get_last_updated($ticket_id) ********************
*** get time of last action **********************************
*************************************************************/
function get_last_updated($ticket_id)
{

	// 1. select actions with ticket_id = $ticket_id
	//$f_query= "SELECT `id` , `ticket_id` , `t_date` FROM `action` WHERE `ticket_id` =$ticket_id ORDER BY `id` DESC";
	if ($ticket_id)
	{
		$f_query= "SELECT `id` , `ticket_id` , `t_date` FROM `action` WHERE `ticket_id` =$ticket_id ORDER BY `t_date` DESC";
		$f_result = mysql_query($f_query) or
			do_error("<p>functions01.inc::get_last_updated(ticket_id:$ticket_id)::mysql_query()", "mysql query failed. f_query was '$f_query'", mysql_error());
		$f_row	= mysql_fetch_array($f_result);
		return $f_row['t_date'];
		// 2. select action_id and action_time
		// 3. sort by action_id
		// 4. return action_id and action_time
	} else {
		do_error("<p>functions01.inc::get_last_updated(ticket_id:$ticket_id)", "variable '\$ticket_id' is void");
	}

}


/********************************************************************************
*** function get_options($table, $field, $selected) *****************************
*** Get list of values from an option. ******************************************
********************************************************************************/
function get_options($table, $field, $selected="--",$index="")
{
	global $debug;
	//$altertext=""; // Text to add TabIndex. // Unused?
	// $lofield=strtolower($field); // Unused?
	// Open MySQL on mysql server with options table
	mysql_open();
	$query = "SELECT value, orden FROM options WHERE options.table_data='$table' ";
	$query = $query . " AND options.field='$field' ORDER BY orden ASC";
	// Debugging info
	if ($debug)
	{
		print "\n".'<p> functions01.inc::get_options()'."\n";
		print '<br>$table = '.$table."\n";
		print '<br>$field = '.$field."\n";
		print '<br>$index = '.$index."\n";
		print '<br>$query = '.$query .'<br><br>'."\n";
	}

	$result	= mysql_query($query) or do_error("get_options()::mysql_query()", 'mysql query failed', mysql_error());
	if (!check_for_rows($query))
	{
		print '<p><FONT CLASS=\"warn\"><B>[no options found]</B></font><BR>';
		return;
	}
	if ($index ) $altertext= "tabindex=\"$index\" ";
	if ($debug)
	{
		print "\n".'<p>functions01.inc::get_options()';
		print '<br>Selectcted Value is '.$selected ."\n";
	}
	while($row = mysql_fetch_array($result))
	{
		if ($debug)
		{
			print "\n".'<p> functions01.inc::get_options()';
			print '<br>Current Value is '.$row['value']."\n";
		}
		( $row['orden'] == $selected ) ? //( $row['value'] == $selected ) ?
			( print '<OPTION VALUE="'.$row['value'].'" SELECTED >'.$row['value'].'</OPTION>') :
			( print '<OPTION VALUE="'.$row['value'].'">'.$row['value'].'</OPTION>' ) ;
	}
	print "</SELECT>\n";
}


/********************************************************************************
*** function get_option_constant($table, $field, $selected) *********************
*** Get list of values from an option. ******************************************
********************************************************************************/
function get_option_constant($table, $selected="--",$index="")
{
	global $debug;
	$altertext=""; // Text to add TabIndex.
	// Open MySQL on mysql server with options table
	mysql_open();
	$query = "SELECT `value`, `field`, `orden` FROM `options` WHERE `table_data`='$table' ORDER BY `orden` ASC";
	// Debugging info
	if ($debug)
	{
		print "\n <p>functions01.inc::get_option_constant()";
		print '<br>$table = '.$table ;
		print '<br>$index= '.$index ;
		print '<br>$selected= '.$selected ." \n";
		print '<br>$query = "'.$query ."\" \n";
	}
	$result	= mysql_query($query) or
		do_error("get_option_constant()::mysql_query()", 'mysql query failed with query ' . $query, mysql_error());
	if (!check_for_rows($query))
	{
		print '</SELECT><br><FONT CLASS=\"warn\"><B>[no options found]</B></font><BR>' ."\n";
		return;
	}
	if ($index ) $altertext= "tabindex=\"$index\" ";
	if ($debug)
	{
		print "<br> functions01.inc::get_option_constant() || Selectcted Values is $selected \n";
	}
	while($row = mysql_fetch_array($result))
	{
		if ($debug)
		{
			print "<br> functions01.inc::get_option_constant() || Current Value is ".$row['value']." \n";
		}
		( $row['value'] == $selected ) ?
			( print '<OPTION VALUE="'.$row['value'].'" SELECTED >'.$row['field'].'</OPTION> ') :
			(print '<OPTION VALUE="'.$row['value'].'">'.$row['field'].'</OPTION>') ;
	}
	print "</SELECT>";
}


/********************************************************************************
*** function get_option_location($table, $field, $selected) *********************
*** Get list of values from an option. ******************************************
********************************************************************************/
function get_option_location($selected="--",$index="")
{
	global $debug;
	$table="ticket";
	$field="location";
	// Open MySQL on mysql server with options table
	mysql_open();
	$query = "SELECT `value`, `field`, `orden` FROM `options` WHERE `table_data`='$table' AND `field`='$field' ORDER BY `orden` ASC";
	// Debugging info
	if ($debug)
	{
		print "\n <p>functions01.inc::get_option_location()";
		print '<br>$table = '.$table ;
		print '<br>$field = '.$field ;
		print '<br>$index = '.$index ;
		print '<br>$query = "'.$query ."\" <br><br> \n";
	}
	$result	= mysql_query($query) or
		do_error("get_option_location()::mysql_query()", 'mysql query failed with query ' . $query, mysql_error());
	if (!check_for_rows($query))
	{
		print '<br><FONT CLASS=\"warn\">get_option_location()::<B>[no options found]</B></font><BR>' ."\n";
		return;
	}
	if ($index ) $altertext= "tabindex=\"$index\" ";
	if ($debug)
	{
		print "<br> functions01.inc::get_option_location() || Selectcted Values is $selected \n";
	}
	while($row = mysql_fetch_array($result))
	{
		if ($debug)
		{
			print "<br> functions01.inc::get_option_location() || Current Values is ".$row['value']." \n";
		}
		( $row['value'] == $selected ) ?
			( print '<OPTION VALUE="'.$row['value'].'" SELECTED >'.$row['value'].'</OPTION> ') :
			(print '<OPTION VALUE="'.$row['value'].'">'.$row['value'].'</OPTION>') ;
	}
	print "</SELECT>";
}


/********************************************************************************
*** function get_option_owner($table, $field, $selected) *****************************
*** Get list of owners from an option. ******************************************
********************************************************************************/
function get_option_owner($selected = "--")
{
	global $debug;
	global $portal_user;
	$portal_user_id = get_id_from_user($portal_user);
	
	$table = "user";
	// Open MySQL on mysql server with options table
	mysql_open();
	$query = "SELECT `id`, `user` FROM `user` WHERE 1 ORDER BY `user` ASC";
	// Debugging info
	if (1) // ($debug)
	{
		print " \n ";
		print "<p> functions01.inc::get_option_owner()" ." \n ";
		print '<br>$table = "'.$table .'"' ." \n ";
		print '<br>$selected = "'.$selected .'"'  ." \n ";
		print '<br>$portal_user = "'. $portal_user.'"'  ." \n ";
		print '<br>$portal_user_id = "'. $portal_user_id.'"' ." \n ";
		print '<br>$query = "'.$query ."\"<br><br> \n ";
	}
	$result	= mysql_query($query)
		or do_error("get_option_owner(table='$table',selected='$selected')::mysql_query()",
				'mysql query failed with query ' . $query, mysql_error());
	if (!check_for_rows($query))
	{
		print '<P><FONT CLASS="warn"><B>[no options found]</B></font><BR>' . "\n";
		return;
	}
	if (1) // ($debug)
	{
		print "<br> functions01.inc::get_option_owner() || selected value is '$selected' \n ";
	}
	while($row = mysql_fetch_array($result))
	{
		if (1) //($debug)
		{
			print "<br> functions01.inc::get_option_owner() || Current Value is ".$row['value']." \n ";
		}
		 $row['id'] == $selected  ? 
		 	print '<OPTION VALUE="'.$row['id'].'" SELECTED >'.$row['user'].'</OPTION> ' : 
			print '<OPTION VALUE="'.$row['id'].'">'.$row['user'].'</OPTION>' ;
	}
}



/*************************************************************
*** function get_owner($id) **********************************
*** get owner name from id ***********************************
*************************************************************/
function get_owner($id)
{
	if ($id)
	{
		$query		= "SELECT `user`,`id` FROM `user` WHERE id='$id'";
		$result		= mysql_query($query) or do_error("get_owner(id:$id)::mysql_query()", 'mysql query failed', mysql_error());
		$row		= mysql_fetch_array($result);
		return 		$row['user'];
	} else {
		do_error("functions01.inc::get_owner($id)","variable '\$is' is empty");
		die;
	}
}


/********************************************************************************
*** function get_pointer($table, $field, $selected) *****************************
*** Get list of values from an option. ******************************************
********************************************************************************/
function get_pointer($table, $field, $selected="")//"--",$index="")
{
	global $debug;
	//$altertext=""; // Text to add TabIndex. // Unused?
	// $lofield=strtolower($field); // Unused?
	// Open MySQL on mysql server with options table
	mysql_open();
	$query		= "SELECT `id`, `".$field."` FROM `".$table."` WHERE 1  "; //ORDER BY `id` ASC ";
	//$query = (($selected != "--") ? ($query . "  ORDER BY `id` ASC") : ($query . "AND `id`='".$selected."' ORDER BY `id` ASC") );
	// Debugging info
	if ($debug)
	{
			print '<p> functions01.inc::get_pointer()';
			print '<br>$table = '.$table ;
			print '<br>$field = '.$field ;
			print '<br>$selected= '.$selected ;
			print '<br>$query = '.$query .'<br><br>';
	}
	$result	= mysql_query($query) or
		do_error("get_pointer()::mysql_query()::query='$query'", 'mysql query failed', mysql_error());
	if (!check_for_rows($query))
	{
		print "</SELECT>";
		print '<p><FONT CLASS=\"warn\"><B>[no options found]</B></font><BR></td>';
		return;
	}
	while($row = mysql_fetch_array($result))
	{
		if ($debug)
		{
			print "\n<!-- <p> functions01.inc::get_pointer() || Current Value is ".$row['value']." ";
			print "\n<br> functions01.inc::get_pointer() || Current ID is ".$row['id']." ";
			print "\n<br> functions01.inc::get_pointer() || Selected  Value is ".$selected." --> \n";
		}
		// We don't want an excesively long field in the drop-down box. Value on database will be "id"
		$short_value = substr($row[$field],0,get_variable('abbreviate_practice')).'...';
		( $row['id'] == $selected ) ? (print '<OPTION VALUE="'.$row['id'].'" SELECTED >'.$short_value.'</OPTION>') :( print '<OPTION VALUE="'.$row['id'].'">'.$short_value.'</OPTION>');
	}
	print "</SELECT>";
}


/*************************************************************
*** function get_single_option($table_data,$value) ***********
*** get owner name from id ***********************************
*************************************************************/
function get_single_option($table_data,$value)
{
	global $debug;
	//$debug="1";
	if (trim($value) == "") $value = "0";
	$query		= "SELECT `field` FROM `options` WHERE `table_data`='$table_data' AND `value`='$value' ";
	if ($debug)
	{
		print " \n <p> functions01.inc::get_single_option()";
		print '<br>$table_data="'.$table_data.'"';
		print '<br>$value="'.$value.'"';
		print '<br>$query="'.$query. "\n ";
	}
	$result		= mysql_query($query) or
		do_error("get_single_option(), table_data='$table_data',value='$value'::mysql_query()", 'mysql query failed', mysql_error());
	$row		= mysql_fetch_array($result);
	if ($debug)
	{
		print " \n <p> functions01.inc::get_single_option()";
		print '<br>$table_data="'.$table_data.'"';
		print '<br>$value="'.$value.'"';
		print '<br>$query="'.$query;
		print '<br>$row[field]="'.$row['field']. "\"\n ";
	}
	return $row['field'];
}


/*************************************************************
*** function get_variable($name) *****************************
*** Gets the value of a variable, stored in a database *******
*************************************************************/
function get_variable($name)
{
	if ($name)
	{
		$query 	= "SELECT name,value FROM settings WHERE name='$name'";
		$result = mysql_query($query) or
					do_error("functions01.inc::get_variable(n:$name)::mysql_query()", 'mysql query failed', mysql_error());
		$row 	= mysql_fetch_array($result);
		return $row['value'];
	} else {
		do_error("functions01.inc::get_variable()","Missing parameter '\$name'");
	}
}


/********************************************************************************
*** function go_back($width=750, $page="main") **********************************
*** Print button "Go Back" ******************************************************
********************************************************************************/
function go_back($page, $text="Go Back to Main Page")
{
	//$url=trim($page) . ".php";
	print " \n <!-- Go-Back Link. Always useful --> \n";
	print '<p><FORM METHOD="post" ACTION="'.$page.'">  '. " \n ";
	print '<INPUT TYPE="submit" VALUE="'.$text.'"></FORM>'. " \n";
}


/***********************************************************************
*** function list_simple_option() **************************************
*** Displays the selected table, in a very simple way ******************
*** any of the table's fields. *****************************************
*** $search means this is a the result of user's search ****************
***********************************************************************/
function list_simple_option($SQL)
{
	global $debug;
	//$debug = 1 ;
	global $per_page;
	//if ($per_page == 0)  $per_page=20;  // Limit results shown to 20 results per page
	$page="action.php";
	// On error, exit.
	//Ternary operator, sort order, ascending/descending
	if  ($debug) // For Debug
	{
		print " \n <p>functions01.inc::list_simple_option()";
		print '<br>$table is '.$GLOBALS['OPTION_TABLE'] ." \n ";
	}
	$order = 'DESC' ;
	$sortby = 'id ';
	$current = 0;
	// SQL query --> How many rows do we have in total?
	$query_max = "SELECT `id` FROM `".$GLOBALS['OPTION_TABLE']."`  WHERE 1 ".$SQL . " " ;   // Select ids from table (fast query)
	if ($debug) // For Debug
	{
		print '<br>$order is '.$order;
		print '<br>$sortby is '.$sortby;
		//print '<br>$ident is '.$ident;
		print '<br>$query_max(maxnum) = '.$query_max." \n ";
	}
	$result_max = mysql_query($query_max) or
		do_error('functions01.inc::list_simple_option()::mysql_query()', 'mysql query failed with $query_max '. $query_max, mysql_error());
	$maxrows=mysql_num_rows($result_max);

	// Build real SQL query
	$query = "SELECT * FROM `".$GLOBALS['OPTION_TABLE']."`  WHERE 1 ".$SQL . " ORDER BY $sortby $order ";

	$result = mysql_query($query) or
			do_error('functions01.inc::list_simple_option()::mysql_query()', 'mysql query failed with $query '. $query, mysql_error());

	// Dual color initialization for information display in "log paper" style.
	$tblcolor = "td_grey";
	print "<!-- Continuing table of Hosts table. Now, the data, bi-colour -->";
	// Fetch results and get highest id
	$high=1;

	if ($debug)
	{
		print " \n <p>functions01.inc::list_simple_option()";
		print '<br>$query = '.$query ;
		print '<br>I\'m out: Starting generation of elements\' table'."   \n ";
	}
	//$index_of_pages=index_elements($maxrows,$current,$per_page,$order,$SQL);
	// (previous line) Index of elements shown. Show $per_page elements at a time
	print '<table  width="750" border="0">'; // Start Results table.

// Search if there is a limited number of fields to show OR if there is special order of fields to be shown.
	global $table_fields;
	which_fields($GLOBALS['OPTION_TABLE']); // filling $table_fields

	if ($debug)
	{
		print '<br>There is  '.sizeof($table_fields).' stuff in which_fields()';
	}
	if (0)
	{
		if ($row = mysql_fetch_array($result)) // As we use this fetch_array, we will use do{} instead of while{}
		{
			// Switch between ASC and DESC sort order.
			//if ( ($_GET['order'] == ('')) OR ($_GET['order'] == ('DESC')) ) $order = "ASC";
		//	else $order = "DESC";
	
			//Print non-clickable headers first
			// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
			$iflds = mysql_num_fields($result) - 1;		// Number of fields
			$irows = mysql_num_rows($result); // number of rows
			// Initialize array with obvious wrong values, to detect errors.
			$fieldnames = array("long", "day","at","the", "races");
			//Request fildnames, in long version
			if($debug)
			{
				print " \n <p>functions01.inc::list_simple_option(): inside table generation. row=mysql_fetch_array(result).";
				print '<br>$order="'. $order .'"' ;
				print '<br>number of fields is $iflds="'. $iflds .'"' ;
				print '<br>number of rows is $irows="'. $irows  .'"';
				print '<br>$table="'. $GLOBALS['OPTION_TABLE']  ."\" \n ";
			}
			$fieldnames = get_fieldname(0, $GLOBALS['OPTION_TABLE']); // 0 => short. !0 => long
			print "  <TR>";  // print row for titles
			for ($i = 0; $i <= $iflds; $i++)
			{
				// This is the field name.
				$ident = (mysql_field_name($result, $i));
				if($debug)
				{
					print " \n <p>functions01.inc::list_simple_option():: Headers. in for()...switch()";
					print '<br> $i="'.$i.'"';
					print '<br> $ident="'.$ident."\" \n";
				}
				// Print headers with a link to field name, for sorting pourposes.
				switch ($i)
				{
				case "0": // Short Fields: id
				case "1": // Short Fields: ticket_id
					print '<TH class="thead">&nbsp;&nbsp;'.$fieldnames[$i].'&nbsp;&nbsp;</TH>'."\n";
				break;
				default: // Other fields:
					print '<TH class="thead" NOWRAP>'.$fieldnames[$i].'</TH>'."\n";
				break;
				}
			}
			if ($debug)
			{
				print " \n <p>functions01.inc::list_simple_option(): Just finished for() switch()...";
				print ' <br>And $i was "'.$i.'" in the end of it';
			}
			print " </TR> \n"; // End of titles' row
			// We already have fetched an array of data. Let's do "do{...}while()" instead of "while(){...}"
		}  else {
			// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
			$iflds = mysql_num_fields($result) - 1;		// Number of fields
			$irows = mysql_num_rows($result); // number of rows
			if($debug)
			{
				print " \n <p>functions01.inc::list_simple_option() -- I'm in the else";
				print '<br>number of fields is '. $iflds ;
				print '<br>number of rows is '. $irows ;
			}
			print '<p class="severity_medium"> No results were found (Num of rows is '.$irows.' )';
		}
	} else {
		if ($debug)
		{
			print '<h3>There is stuff in which_fields() : ' . sizeof($table_fields) .'</h3>';
			$i=0;
			while( $element = each($table_fields))
			{
				$tempp_value=1;	 // [1] contains the proper 'value'. [0] contains 'order'
				print '<br>element num.'.$i. ' = ' .$element[$tempp_value];//[1];
				$i++;
			}
		}
		if ($row = mysql_fetch_array($result)) // As we use this fetch_array, we will use do{} instead of while{}
		{
			if ( ($_GET['order'] == ('')) OR ($_GET['order'] == ('DESC')) ) $order = "ASC";
			else $order = "DESC";
			//Print clickable headers first
			// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
			$iflds = mysql_num_fields($result) - 1;		// Number of fields
			$irows = mysql_num_rows($result); // number of rows
			// Initialize array with obvious wrong values, to detect errors.
			$fieldnames = array("long", "day","at","the", "races");
			//Request fildnames, in long version
			if($debug)
			{
				print " \n <p>functions01.inc::list_simple_option(): inside table generation. row=mysql_fetch_array(result). Print SOME fields";
				print '<br>$order="'. $order .'"' ;
				print '<br>number of fields is $iflds="'. $iflds .'"' ;
				print '<br>number of rows is $irows="'. $irows  .'"';
				print '<br>$table="'. $GLOBALS['OPTION_TABLE']  ."\" \n ";
			}
			$fieldnames = get_fieldname(0, $GLOBALS['OPTION_TABLE']); // 0 => short. !0 => long
			print "  <TR>";  // print row for titles
	
			while( $element = each($table_fields)) // [1] contains the proper 'value'. [0] contains 'order'
			{
				// This is the field name.
				$ident = (mysql_field_name($result, $element[1]));
				// Print headers with a link to field name, for sorting pourposes.
				if($debug)
				{
					print " \n <p>functions01.inc::list_simple_option():: Headers. in while{ switch () }";
					print '<br> $i="'.$i.'"';
					print '<br> $ident="'.$ident."\" \n";
				}
				print '<TH class="thead" NOWRAP>'.$fieldnames[$element[1]].'</TH>'."\n";
			}
	
			print " </TR>"; // End of titles' row
			// We already have fetched an array of data. Let's do "do{...}while()" instead of "while(){...}"
			do { //Now, it's "display-the-data" time,  One element per line. :)
				// Make $high held the highest value of $row[0] a.k.a. $row["id"]
				$high > $row[0] ? $high = $high : $high = $row[0];
				// First, get a link in the 'id' column. Then, print the rest.
				/*  */
				reset($table_fields);
				while( $element_data = each($table_fields)) // [1] contains the proper 'value'. [0] contains 'order'
				{
					if ($debug)
					{
						print "<br>current element_data is $element_data[1]";
					}
	
					switch ($element_data[1])
					{
					case "0": // Short Field: Num
						print '<TR ><TD class="'.$tblcolor.'"  NOWRAP >#'.$row[0];
						print '</TD>'."\n";
					break;
					case (1): // ticket_id
						print '<TD class="'.$tblcolor.'" NOWRAP >'.$row[1];
						print '</TD>'."\n";
					break;
					case (2): // date.
						print '<TR><TD class="'.$tblcolor.'"  NOWRAP >'.$row[2];
						print '</TD>'."\n";
			//			$date_value = date("Y-m-d H:i:s"); // Only current date in "date added"
					break;
					case (3): // 2-line inputut field
						print '<TD class="'.$tblcolor.'" >'.$row[$element_data[1]];
						print '</TD>'."\n";
					break;
					case (4): // user. Hidden element
						print '<TD class="'.$tblcolor.'" NOWRAP>'.get_owner($row[$element_data[1]]);
						print '</TD>'."\n";
					break;
					case (5): // action_type. Hidden element
						print '<TD class="'.$tblcolor.'" NOWRAP>'.get_single_option("action_description",$row[$element_data[1]]);
						print '</TD>'."\n";
					break;
					}
				}
				print "  </TR> \n";/* */
				// Ternary operator for bi-color rows, log-paper style.
				$tblcolor == "td_grey" ? 	$tblcolor = "td_darkgrey" : $tblcolor = "td_grey";
			 } while($row = mysql_fetch_array($result));
		}  else
		{
			// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
			$iflds = mysql_num_fields($result) - 1;		// Number of fields
			$irows = mysql_num_rows($result); // number of rows
			if($debug)
			{
				print " \n ".'<p>functions01.inc::list_simple_option() || I\'m in the else<br>';
				print '<br>number of fields is '. $iflds ;
				print '<br>number of rows is '. $irows ;
			}
			print '<p class="severity_medium"> No results were found (Num of rows is '.$irows.' )';
		}
	}
	print "</table>\n <br>"; // Finish table
	//print $index_of_pages; // This substitutes previous line. Hopefuly, now index with proper ASC/DESC will work.
	if($debug)
	{
		print'<p>functions01.inc::list_simple_option(): Exiting function, with $high as '. $high;
	}
	return $high; //Yeah, we will be high after all this code.
}


/*************************************************************
*** function list_users() ************************************
***  list users  *********************************************
*************************************************************/
function list_users()
{
	$query = 'SELECT * FROM user';
	$result = mysql_query($query) or
		do_error('functions01.inc::list_users()::mysql_query()', 'mysql query failed', mysql_error());

	if (!check_for_rows('SELECT id FROM user')) { print '<B>[no users found]</B><BR>'; return; }

	print '<TABLE BORDER="0"> <th class="td_header">Users</th>
		<TR>
		<TD><B>ID&nbsp;&nbsp;&nbsp;</B></TD>
		<TD><B>User&nbsp;&nbsp;&nbsp;</B></TD>
		<TD><B>Description&nbsp;&nbsp;&nbsp;</B></TD>
		<TD><B>User Type&nbsp;&nbsp;&nbsp;</B></TD>
		</TR>';
		//<TD><B>Administrator | User | Read-Only&nbsp;&nbsp;&nbsp;</B></TD>
	while($row = mysql_fetch_array($result))
	{
		switch ($row['admin'])
		{
			case "0":
				$row['admin'] = 'Read-Only';
			break;
			case "1":
				$row['admin'] = 'User';
			break;
			case "2":
				$row['admin'] = 'Administrator';
			break;
		}
		print "<TR><TD><A HREF=\"config.php?mode=id&id=$row[id]\">#$row[id]</A></TD>
			<TD>$row[user]</TD><TD>$row[info]</TD>	<TD>$row[admin]</TD></TR>\n";
	}
	print '</TABLE><BR>';
}


/*******************************************************************
*** function location_config() *************************************
*** Add/Modify locations *******************************************
*******************************************************************/
function location_config($value)
{
	global $debug;
	$max = 0;
	if ($debug)
	{
		print '<p>functions01.inc::location_config()<br>$value='.$value;
	}
	switch($value)
	{
		case 0: // Add Location
			if ($debug) print '<br> case 0';
			print'<BR><TABLE BORDER="0">
			<FORM METHOD="POST" ACTION="config.php?addloc=true&go=true">';
			print'<TR> <TD CLASS="td_label">Location:</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" VALUE="'.$_POST['frm_loc'].'" NAME="frm_loc"></TD></TR>';
			print'<TR><TD></TD><TD><INPUT TYPE="submit" VALUE="Add Location"></TD></TR>	</FORM></TABLE>';
		break;
		case 1: // Add Location
			if ($debug) print '<br> case 1';
			print '<FONT CLASS="header">Locations Management</FONT><BR><BR><TABLE BORDER="0">
			<FORM METHOD="POST" ACTION="config.php?mode=addloc&go=true">';
			print'<TR><TD CLASS="td_label">Location (text):</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" NAME="frm_loc"></TD></TR>';
			// Get previous max value from existing locations
			print'<TR><TD CLASS="td_label">Value (number):</TD>
				<TD><HIDDEN MAXLENGTH="2" SIZE="5" TYPE="text" NAME="frm_value" VALUE="'.($max+1).'"> MAX+1</TD>	</TR>';

			print'<TR><TD ><INPUT TYPE="submit" VALUE="Add Location"></FORM></TD> ';
			print'<TD align="right"> <FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></TD>';
			print '</TR>	</FORM></TABLE>';
			print '<BR>';
			if (!check_for_rows("SELECT `id` FROM `options`"))
			{
				print '<p><B>[no Locations found]</B><BR>'; return;
			} else {
				$query = "SELECT `id` , `table_data` , `field` , `value` FROM `options` ";
				$query = $query . " WHERE 1 AND `table_data` = 'location' ORDER BY `value` ASC";
				$result = mysql_query($query) or do_error('config.php::list_settings', 'mysql_query() failed', mysql_error());
				// Show list of existing locations
				print "<h3>Existing Locations</h3>";
				print '<TABLE><TR ><TD class="thethead" colspan="2">ID | Location description</TD></TR>';
				while($row = mysql_fetch_array($result))
				{
					print '<TR>';
					print '<TD>'.$row['value'].'</TD><TD>'.$row['field'].'</TD>';
					print '</TR>';
				}
			}
		break;
		case 3: // Show Location data, ready to be edited
			if ($debug) print '<br> case 3';
			print '<FONT CLASS="header">Edit Location</FONT><BR><BR><TABLE BORDER="0">';
			print '<FORM METHOD="POST" ACTION="config.php?mode=locedit"><INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$row[id].'">';
			print '<TR>
				<TD CLASS="td_label">Location:</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" VALUE="'.$row[field].'" NAME="frm_loc"></TD>
				</TR>';
			print '<TR>
				<TD CLASS="td_label">Value:</TD>
				<TD><INPUT MAXLENGTH="30" SIZE="40" TYPE="text" NAME="frm_value"></TD>
				</TR>';
			print '<TR>
				<TD CLASS="td_label">Remove Location:</TD>
				<TD ALIGN="left"><INPUT TYPE="checkbox" VALUE="yes" NAME="frm_remove"> (check to remove location)</TD>
			</TR>';
			print '<TR>
				<TD></TD>
				<TD><INPUT TYPE="submit" VALUE="Apply"></TD>
			</TR>
			</FORM></TABLE>';
		break;
		default :
			if ($debug) print '<br> case default';
		break;
	}
}


/*******************************************************************
*** function manager_options() *****************************
*** Build the query for the $action = 'updat' case *****************
*******************************************************************/
function manager_options($action, $query="")
{
	global $debug;
	if ($debug)
	{
		print '<p> functions01.inc::make_insert_query()';
		print '<br>$action = "' .$action .'"';
		print '<br>$query = "' .$query .'"';
		print '<br>this is a test, manager option';
		print '<p><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM><p>';
	}
	if (!$query)
	{
		print '<p>$action is '. $action;
		print '<p><FORM METHOD="post" ACTION="config.php?mode=manager&go=true">  '. " \n ";
		print '<p><TEXTAREA NAME="frm_query" COLS="36" ROWS="2" tabindex="1" ></TEXTAREA>';
		print '<table><tr><td>';
		print '<INPUT TYPE="submit" VALUE="Submit your query"></FORM>'. " \n";
		print '</td><td>';
		print '<FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM>';
		print '</td></tr></table>';
	} else {
		print '<p>Your query was "'.$query .'"';
		//go_back("config");
	}
}


/*******************************************************************
*** function make_insert_query($table) *****************************
*** Build the query for the $action = 'updat' case *****************
*******************************************************************/
function make_insert_query($table, $type,$id,$visit=0,$aid=0)
{
	// On error, exit .
	if (! $table )
	{
		print '<P class="severity_medium"> Error! no variable given to make_insert_query($table)!';
		return 0;
	}
	// Define global variables for data
	global $link_next ;
	global $mysql_db_next;
	global $debug;
	$fields = mysql_list_fields($mysql_db_next, $table, $link_next); // retrieve list of fields
	$columns = mysql_num_fields($fields);
	if ($debug)
	{
		print '<p> functions01.inc::make_insert_query()';
		print '<br>$table ='.$table;
		print '<br>$type='.$type;
		print '<br>$id='.$id;
		print '<br>$visit='.$visit;
		print '<br>$aid='.$aid. "\n";
	}
	switch ($type)
	{
	case "insert": // Insert $element
		$i = 1; // We don't want to put the id value (auto fileld/increment field)
		$query = "INSERT INTO $table ("; // Start the query for INSERT
		$temp_query=") VALUES("; // start the subquery with the values
		for ($i ; $i < $columns; $i++)  // Generate the queries with a loop
		{
			$query = $query . "`".trim(mysql_field_name($fields, $i)) ."`,"; // Build field names' list with no extra spaces
			$frm_temp_value = "frm_" .strtolower(mysql_field_name($fields, $i)); // Build  field values' list
			if ($debug)
			{
				print '<br>'.(mysql_field_name($fields, $i)).' = '. $frm_temp_value .' with value '.trim($_POST[$frm_temp_value] ).' .';
			}

			switch ($i) //Build field Values in special way: dates
			{
				case (1): // Ticket date: t_date
					$temp_query = $temp_query . "'".date("Y-m-d H:i:s") ."',"     ;	//...  with no extra spaces
				break;
				case (2): // Problem starts: $frm_problemstart
					$frm_problemstart = $_POST['frm_year_problemstart'] .'-'. $_POST['frm_month_problemstart'] .'-'. $_POST['frm_day_problemstart'] .' ';
					$frm_problemstart = $frm_problemstart  . $_POST['frm_hour_problemstart'] .':'. $_POST['frm_minute_problemstart'] .':00';
					$temp_query = $temp_query . "'".trim($frm_problemstart) ."',"     ;	//...  with no extra spaces
				break;
				case (3): // Problem ends: $frm_problemend
					$frm_problemend   = $_POST['frm_year_problemend'] .'-'. $_POST['frm_month_problemend'] .'-'. $_POST['frm_day_problemend'] .' ';
					$frm_problemend   = $frm_problemend   + $_POST['frm_hour_problemend'] .':'.	$_POST['frm_minute_problemend'] .' :00';
					$temp_query = $temp_query . "'".trim($frm_problemend) ."',"     ;	//...  with no extra spaces
				break;
				default:
					$temp_query = $temp_query . "'".trim($_POST[$frm_temp_value] )."',"     ;	//...  with no extra spaces
				break;
			}
		}
		$query = substr($query,0, (strlen($query) -1) ) ; // Remove trailing 's' from $query.
		$temp_query = substr($temp_query,0, (strlen($temp_query) -1) ) ; // Remove trailing 's' from $temp_query.
		$query = $query . $temp_query .")"; // Finish up the query.
	break;
	case "update":  //update $Element.
		$i = 1 + $visit; // We want to leave alone the id value (it's the zero element)	or pid and vid in Visits.
		// Dinamicaly build field and frm_* names
		if ($visit)
		{
			$query = "SELECT * FROM $table WHERE vid='$aid'"; //pid='$id' AND vid='$aid'";
		}
		else
		{
			$query = "SELECT * FROM $table WHERE ID='$id'";
		}
		$result = mysql_query($query) or
					do_error('functions01.inc::make_insert_query()::mysql_query()', 'mysql query failed', mysql_error());
		$row = mysql_fetch_array($result);
		$query = "UPDATE $table SET ";
		for ($i ; $i < $columns; $i++)
		{
			$query = $query ."`".trim(mysql_field_name($fields, $i)). "`='"  ; // Build field names' list with no extra spaces
			$frm_temp_value = "frm_" .strtolower(mysql_field_name($fields, $i)); // Build  field values' list
			$query = $query . trim($_POST[$frm_temp_value] )."'"     ; // ... with no extra spaces
			if ($i < ($columns-1)) $query =  $query. "," ; // Finish up the query.
		}
		if ($visit) $query = $query ." WHERE vid='".$_POST[frm_vid]."'";
		else $query = $query ." WHERE id='".$_POST[frm_id]."'"; // This '$_POST....' is the table id, not '$id'

		if ($debug)
		{
			print "<p>The dinamically build \$query on functions01.inc::make_insert_query()::update<br>";
			print $query."\n";
		}
	break;
	case "close":  //close $Element.
		$frm_closefield = "frm_" .strtolower($GLOBALS['CLOSED_FIELD']);
		$query = "UPDATE `$table` SET `".$GLOBALS['CLOSED_FIELD']."` = '".$_POST[$frm_closefield]."',";
		$query = $query . "`".$GLOBALS['STATUS_FIELD']."` = '". $GLOBALS['STATUS_CLOSED']."' ";
		$query = $query ." WHERE id='".$_POST[frm_id]."' LIMIT 1"; // This '$_POST....' is the table id, not '$id'
		if ($debug)
		{
			print "<p>The dinamically build \$query on functions01.inc::make_insert_query()::update<br>";
			print "<br>query is ''$query'' \n ";
		}
	break;
	}
	if ($debug)
	{
		print '<p>Finishing functions01.inc::function make_insert_query('.$table.')::"'.$type.'"';
		print '<br>$query is :<br>"'.$query."\" \n";
	}
	return $query;
}


/*************************************************************
*** function mysql_open() ************************************
*** Opens MySQL Database using global vars *******************
*************************************************************/
function mysql_open()
{
	global $link;
	global $mysql_host;
	global $mysql_user;
	global $mysql_passwd;
	global $mysql_db;

	$link = mysql_connect("$mysql_host", "$mysql_user", "$mysql_passwd")
		or do_error('functions01.inc::mysql_open()', 'mysql_connect() failed', mysql_error());
    mysql_select_db("$mysql_db") or do_error('functions01.inc::mysql_open()',
		'mysql_select_db() failed', mysql_error());
}


/****************************************************************************
*** function mysql_open_next( $db_next,   $user_next='',  $passwd_next='') **
*** Opens 2nd MySQL Database/table using referenced/global vars *************
****************************************************************************/
function mysql_open_next( $db_next='',   $user_next='',  $passwd_next='')
{
	// Values to Global vars
	global $mysql_db_next;
	global $mysql_user_next;
	global $mysql_passwd_next;
	global $mysql_host_next;
	global $link_next;

	// use default : conf.inc.php
	if ($user_next != '')  $mysql_user_next = $user_next;
	// else, use the default
	if ($passwd_next != '')  $mysql_passwd_next = $passwd_next;
	// else, use the default
	if ($db_next != '')  $mysql_db_next=$db_next ;
	else return (1);

	$link_next = mysql_connect("$mysql_host_next", "$mysql_user_next", "$mysql_passwd_next")
		or do_error('functions01.inc::mysql_open_next()', 'mysql_connect() failed', mysql_error());
	mysql_select_db("$mysql_db_next") or do_error('functions01.inc::mysql_open_next()',
		'mysql_select_db() failed', mysql_error());

	return $link_next;
}


/*************************************************************
*** function mysql_shutdown() ********************************
*** close mysql connection properly **************************
*************************************************************/
function mysql_shutdown()
{
	global $link;
	mysql_close($link);
}


/*************************************************************
*** function mysql_shutdown_next() ***************************
*** close mysql_next connection properly *********************
*************************************************************/
function mysql_shutdown_next()
{
	global $link_next;
	mysql_close($link_next);
}


/*************************************************************
*** function powered($graphs=0) ******************************
*** Print end of HTML page. **********************************
*** Optionaly, it can print the "powered by" icons :) ********
*************************************************************/
function powered($graphs=0)
{
	global $mysql_db;
	global $mysql_user;
	global $mysql_passwd;
	print " \n  <!-- End of webpage: footer -->";
	mysql_open("$mysql_db", "$mysql_user", "$mysql_passwd");
	print '<hr><!--<div align="center">Powered by:</div>-->
	<table width="100%">
	<tbody>
	<tr>
		<td align="center"><a href="'.get_variable('organization_website').'" target="_blank">'.get_variable('organization').'</a></td>' ." \n ";
	if ($graphs)
	{
		print '
		<td><a href="http://www.redhat.com" target="_blank">
				<img src="/icons/poweredby.png" alt="Red Hat Linux" width=88 height=31 border=0></a>
				<a href="http://www.mandrake-linux.com" target="_blank">
				<img src="/icons/mdkpowered.jpg" alt="Mandrake Linux" width="175" height="50" border="0"></a>
		</td>' ." \n ";
		'<td><a href="http://www.apache.org" target="_blank">
				<img src="/icons/apache_pb.png" alt="Apache" width="259" height="32" border="0"></a>
		</td>' ." \n ";
		'<td><a href="http://www.mysql.org" target="_blank">
				<img src="/icons/mysql-powered.png" alt="MySQL" width="82" height="32" border="0"></a>
		</td>' ." \n ";
		'<td><a href="http://www.php.net" target="_blank">
				<img src="/icons/php-powered.png" alt="PHP" width="88" height="31" border="0"></a>
		</td>
		</td>' ." \n ";
	}
	print '</tr>	</tbody>	</table>' ." \n ";
	print '<!-- End of HTML page --> </BODY></HTML>' ." \n ";
	die;
}


/**************************************************************
*** function send_email($email_text) **************************
*** Send email from phpTicket *********************************
**************************************************************/
function send_email($email_text)
{
   $from = get_variable("admin_email");
   $to = get_variable("admin_email");
   $title = "[Ticket] New Support Ticket ";
//$body = <<< emailbody
//Dear Sysadmins,

//This email confirms that php-email works

//Thank you,
//phpTicket New Generation.

//emailbody;
   // $success = mail($to,$title,$body, "From:$from\r\nReply-To:$from");
   $success = mail($to,$title,$email_text, "From:$from\r\nReply-To:$from");
}


/**************************************************************
*** function set_htmlheader($title) **************************
*** Prints HTML headers, to formaly start to build the page ***
**************************************************************/
function set_htmlheader($title, $refresh=0, $url=0)
{
	// $title = ucfirst($title); // Capitalize first letter
	global $debug;
	if ($debug) {
		print '<p>functions01.inc::set_htmlheader("'.$title.'","'.$refresh.'","'.$url.'")';
	}
	print '<html><head>
		<title>'.$title.'</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">';
	if ($refresh)
	{
		print '<META HTTP-EQUIV="refresh" content="'.$refresh.'; URL='.$url.'">';
	}
	print '<LINK REL="StyleSheet" HREF="default.css" TYPE="text/css">';
	
}


/*************************************************************
***  function show_simple_data($id,$page,$start_id) ****
*** Show the selected $element, building all dinamicaly ******
*************************************************************/
function show_simple_data($id,$start_id=0)
{
	global $debug;

	$name = $GLOBALS['ELEMENT']	;

	//For Debugging
	if ($debug)
	{
		print " \n <p>functions01.inc::show_simple_data(): Showing $name $id \n";
		print "<br> id = $id";
		print "<br> start_id = $start_id \n ";
	}

	/* sanity check */
	if ($id == '' OR $id <= 0 OR !check_for_rows("SELECT * FROM ".$GLOBALS['MAIN_TABLE']." WHERE id='$id'"))
	{
		print "<p>functions01.inc::show_simple_data()<br>Invalid $name ID: '$id'<br>\n";
		simple_search_by_element_num($GLOBALS['ELEMENT']);
		return;
	}
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $GLOBALS['MAIN_TABLE']); // 0 => short. !0 => long
	// Build the SQL query
// This will be the fieldname --> $fieldnames[$i]
	// Retrieve the field-names of $table from the database $mysql_db_next
	global $link_next ;
	global $mysql_db_next;
	$fields = mysql_list_fields($mysql_db_next, $GLOBALS['MAIN_TABLE'], $link_next);
	$columns = mysql_num_fields($fields);

	// Build the SQL query
	$query = "SELECT * FROM ".$GLOBALS['MAIN_TABLE']." WHERE ID='$id'";
	if ($debug)
	{
		print " \n <br>show_simple_data(): query is '$query' \n";
	}
	$result = mysql_query($query) or
		do_error('functions01.inc::show_simple_data():mysql_query()', 'mysql query failed', mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0" bgcolor="white"> <tr><td  width="25%" valign="top">'; 	  print "\n";
	print '</td></tr><tr><td  valign="top" width="70%">';	  print "\n";
	print '<TABLE BORDER="0" bgcolor="white" width="100%">';
	print '<thead><p class="severity_medium">'.$name.'\'s Information:</p></thead>';
	// $start_id used to hide all index fields
	for ($i = ($start_id+1); $i < $columns; $i++)  // i=0 --> id. Let's hide this field.
	{
		print '<TR>';
		print ' <TD CLASS="td_greylabel" width="25%">'.$fieldnames[$i].':';
		if ($debug)
		{
			print " \n <br>show_simple_data(): \$i is '$i' \n";
			print " \n <br>show_simple_data(): \$fieldnames[\$i] is '$fieldnames[$i]' \n";
			print " \n <br>show_simple_data(): \$row[\$i] is '$row[$i]' \n";
		}
		print '</TD>';  print "\n";
		print ' <TD width="75%">';
		switch ($i)
		{
			case (7):
				if ($debug)
				{
					print " \n <br>get_single_option is '".get_single_option("status", $row[$i])."'\n";
				}
				print get_single_option("status", $row[$i]);
			break;
			case (8):
				print get_owner($row[$i]);
			break;
			case (9):
				print get_single_option("severity", $row[$i]);
			break;
			case (10):
				print get_single_option("location", $row[$i]);
			break;
			default:
				print $row[$i];
			break;
		}
		print ' </TD></TR>';	  print "\n";
	}
	print '</TABLE>';	 print "\n";
	print '</td></tr></TABLE>';	  print "\n";
	mysql_shutdown_next();
	return 0;
}


/*************************************************************
*** function show_stats() ************************************
*** Show Stats (Delete after config is done???) **************
*************************************************************/
function show_stats()
{
	$user_in_db = check_for_rows('SELECT * FROM user');

	if ($user_in_db == 0) $user_in_db = '[none]';

	print '<TABLE BORDER="0"><TR>';
	print '<TR><TD CLASS="td_label">Users in database:</TD><TD ALIGN="left">&nbsp;&nbsp;&nbsp;'.$user_in_db.'</TD></TR>';
	print '<TR><TD CLASS="td_label">Current User:</TD><TD ALIGN="right">';
	print $_SESSION['ticket_username'];
	//print $_SESSION['ticket_user_is_admin'] ? ' [administrator]</TD></TR></TABLE>' : ' [user]</TD></TR></TABLE>';
	print ($_SESSION['ticket_user_is_admin']>1) ? ' [administrator]' : ' [user]';
	print ($_SESSION['ticket_user_is_admin']<1) ? '[read-only]</TD></TR></TABLE>' : '</TD></TR></TABLE>';
}


/*************************************************************
***  function  simple_search_by_element_num($element) ********
*** Display little "Search by $element Number" box ***********
*************************************************************/
function simple_search_by_element_num($element)
{
	print '<FORM METHOD="post" ACTION="show.php">' ."\n";
	print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
	print '<TR><TD VALIGN="top" CLASS="td_label">Search by '.$element.' Number (Num.) &nbsp;</TD>' ."\n";
	print '<TD><INPUT TYPE="text" SIZE="4" MAXLENGTH="4" VALUE="" NAME="id"></TD>' ."\n";
	print '<TD><INPUT TYPE="submit" VALUE="Go"></TD></TR>' ."\n";
	print '</TABLE></FORM>' ."\n";
}


/********************************************************************************
*** function transaction_management($id, $table, $type, $reason,$portal_user) ***
*** Do the Transaction Management dance. ****************************************
********************************************************************************/
/*function transaction_management($id, $table, $type, $reason,$portal_user)
{
	global $debug;
	global $mysql_db_next;
	$old_data="";

	if ($debug) print "<p>functions01.inc::transaction_management(). <br>$id, <br>$table, <br>$type, <br>$reason, <br>$portal_user";
	// sanity check . id=0 means new $ELEMENT
	if ($id == '') // OR $id < 0 OR !check_for_rows("SELECT * FROM $table WHERE id='$id'"))
	{
		print "<p CLASS=\"severity_high\">Invalid $type ID: '$id'<br>";  // Changed Element to $something *************
		return;
	}
	switch ($type)
	{
		case "delet": // Delete data. Same as update.
		case "updat": // Update data
			$message_dbg="Transaction Management: $type<br>";
			// Transaction Management: log user, date, time, changes.
			$query = "SELECT * FROM $table WHERE id='$id'";
			$result = mysql_query($query) or do_error('functions01.inc::transaction_management('.$type.')',
				'mysql_query() failed', mysql_error());
			// Thank you to iarenaza@escomposlinux.org for solving a bug here. Long life to the Lord Oscuro!
			$row = mysql_fetch_array($result, MYSQL_ASSOC);   // The second parameter is needed to retrieve the proper row info (key, value)
			// $row = mysql_fetch_row($result); // This doesn't provide field names (id, Description,IP_Address...). Only field numbers (0,1,2...)
			while (list($key, $val)  =  each($row))	$old_data = $old_data . ' '. $key.' : '.$val .' ||';
		break;
		case "add": // Add data
			$message_dbg="Transaction Management: add<br>";
			$old_data=" ";
		break;
		case "deletVisit": // Delete data. Same as update.
		case "updatVisit": // Update data
			$message_dbg="Transaction Management: $type<br>";
			// Transaction Management: log user, date, time, changes.
			$query = "SELECT * FROM $table WHERE vid='$id'";
			$result = mysql_query($query) or do_error('functions01.inc::transaction_management('.$type.')',
				'mysql_query() failed', mysql_error());
			// Thank you to iarenaza@escomposlinux.org for solving a bug here. Long life to the Lord Oscuro!
			$row = mysql_fetch_array($result, MYSQL_ASSOC);   // The second parameter is needed to retrieve the proper row info (key, value)
			// $row = mysql_fetch_row($result); // This doesn't provide field names (id, Description,IP_Address...). Only field numbers (0,1,2...)
			while (list($key, $val)  =  each($row))	$old_data = $old_data . ' '. $key.' : '.$val .' ||';
		break;
		case "addVisit": // Add data
			$message_dbg="Transaction Management: add<br>";
			$old_data=" ";
		break;
	}
	// Let's close remote database and open the transactional database.
	mysql_shutdown_next();
	mysql_open();
	// Let's add the data to 'changes' table
	$query  = "INSERT INTO changes (user,date_time,orig_db,orig_table,orig_data,comments)
		VALUES('$portal_user','".date("H:i, jS F, Y")."','$mysql_db_next','$table','$old_data','$reason')";
	if ($debug )  print '<p>functions01.inc::transaction_management()<br>$old_data = '.$old_data.'<br> $query = '.$query.'<br>';
	$result = mysql_query($query) or do_error('functions01.inc::transaction_management(adding reason and transaction data)::
	mysql_query()', 'mysql query failed', mysql_error());
	mysql_shutdown();
	mysql_open_next($mysql_db_next);
	return;
}*/


/*************************************************************
*** function user_config($value=0) ***************************
*** User configuration ***************************************
*************************************************************/
function user_config($value=0, $row="")
{
	global $debug;
	if ($debug) print '<p>functions01.inc::user_config()<br>$value='.$value.' || $row='.$row;
	switch($value) // Case 2 is the same as 0
	{
		case 0: // Add user
			if ($debug) print '<br> case 0';
			print'<BR><TABLE BORDER="0">
			<FORM METHOD="POST" ACTION="config.php?adduser=true&go=true">';
			print'<TR> <TD CLASS="td_label">User:</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" VALUE="'.$_POST['frm_user'].'" NAME="frm_user"></TD></TR>';
			print'<TR><TD CLASS="td_label">Password</TD>
				<TD><INPUT MAXLENGTH="30" SIZE="40" TYPE="password" NAME="frm_passwd"></TD></TR>';
			print'<TR><TD CLASS="td_label">Confirm Password: &nbsp;&nbsp;</TD>
				<TD><INPUT MAXLENGTH="40" SIZE="40" TYPE="password" NAME="frm_passwd_confirm"></TD></TR>';
			print'<TR><TD CLASS="td_label">User Type:</TD>
				<TD ALIGN="right">';
			print '<INPUT type="radio" name="frm_admin" value="0"> Read-Only
					<INPUT type="radio" name="frm_admin" value="1"> Regular User
					<INPUT type="radio" name="frm_admin" value="2"> Administrator
				</TD></TR>';
			print '<TR><TD CLASS="td_label">Info:</TD>
				<TD><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$_POST['frm_info'].'" NAME="frm_info"></TD></TR>';
			print'<TR><TD><INPUT TYPE="submit" VALUE="Add User"></FORM></TD>';
			print'<TD align="right"> <FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel"></TD>';
			print '</TR>	</FORM></TABLE>';
		break;
		case 1:
			if ($debug) print '<br> case 1';
			print '<FONT CLASS="header">Add User</FONT><BR><BR><TABLE BORDER="0">
			<FORM METHOD="POST" ACTION="config.php?mode=adduser&go=true">';
			print'<TR><TD CLASS="td_label">User:</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" NAME="frm_user"></TD></TR>';
			print'<TR><TD CLASS="td_label">Password</TD>
				<TD><INPUT MAXLENGTH="30" SIZE="40" TYPE="password" NAME="frm_passwd"></TD>	</TR>';
			print'<TR><TD CLASS="td_label">Confirm Password: &nbsp;&nbsp;</TD>
				<TD><INPUT MAXLENGTH="40" SIZE="40" TYPE="password" NAME="frm_passwd_confirm"></TD></TR>';
			print'<TR><TD CLASS="td_label">User Type:</TD>
				<TD ALIGN="right">';
			print '<INPUT type="radio" name="frm_admin" value="0"> Read-Only
					<INPUT type="radio" name="frm_admin" value="1"> Regular User
					<INPUT type="radio" name="frm_admin" value="2"> Administrator
				</TD></TR>';
			print'<TR><TD CLASS="td_label">Info:</TD>
				<TD><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" NAME="frm_info"></TD></TR>';
			print'<TR><TD><INPUT TYPE="submit" VALUE="Add User"></FORM></TD>';
			print'<TD align="right"> <FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel"></TD>';
			print '</TR>	</FORM></TABLE>';
		break;
		case 2:
			if ($debug) print '<br> case 2';
		// Same as 0
			print'<BR><TABLE BORDER="0">
			<FORM METHOD="POST" ACTION="config.php?adduser=true&go=true">';
			print'<TR> <TD CLASS="td_label">User:</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" VALUE="'.$_POST['frm_user'].'" NAME="frm_user"></TD></TR>';
			print'<TR><TD CLASS="td_label">Password</TD>
				<TD><INPUT MAXLENGTH="30" SIZE="40" TYPE="password" NAME="frm_passwd"></TD></TR>';
			print'<TR><TD CLASS="td_label">Confirm Password: &nbsp;&nbsp;</TD>
				<TD><INPUT MAXLENGTH="40" SIZE="40" TYPE="password" NAME="frm_passwd_confirm"></TD></TR>';
			print'<TR><TD CLASS="td_label">User Type:</TD>
				<TD ALIGN="right">';
			print '<INPUT type="radio" name="frm_admin" value="0"> Read-Only
					<INPUT type="radio" name="frm_admin" value="1"> Regular User
					<INPUT type="radio" name="frm_admin" value="2"> Administrator
				</TD></TR>';
			print '<TR><TD CLASS="td_label">Info:</TD>
				<TD><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$_POST['frm_info'].'" NAME="frm_info"></TD></TR>';
			print'<TR><TD><INPUT TYPE="submit" VALUE="Add User"></FORM></TD>';
			print'<TD align="right"> <FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel"></TD>';
			print '</TR>	</FORM></TABLE>';
		break;
		case 3: // Show User data, ready to be edited
			if ($debug) print '<br> case 3';
			print '<FONT CLASS="header">Edit User</FONT><BR><BR><TABLE BORDER="0">';
			print '<FORM METHOD="POST" ACTION="config.php?mode=edit"><INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$row[id].'">';
			print '<TR>
				<TD CLASS="td_label">User:</TD>
				<TD><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" VALUE="'.$row[user].'" NAME="frm_user"></TD>
				</TR>';
			print '<TR>
				<TD CLASS="td_label">Password</TD>
				<TD><INPUT MAXLENGTH="30" SIZE="40" TYPE="password" NAME="frm_passwd"></TD>
				</TR>';
			print '<TR>
				<TD CLASS="td_label">Confirm Password: &nbsp;&nbsp;</TD>
				<TD><INPUT MAXLENGTH="40" SIZE="40" TYPE="password" NAME="frm_passwd_confirm"></TD>
			</TR>';
			print'<TR><TD CLASS="td_label">User Type:</TD>
				<TD ALIGN="right">';
			switch ($row['admin'])
			{
				case "0":
					print '<INPUT type="radio" name="frm_admin" value="0" checked> Read-Only
					<INPUT type="radio" name="frm_admin" value="1"> Regular User
					<INPUT type="radio" name="frm_admin" value="2"> Administrator';
				break;
				case "1":
					print '<INPUT type="radio" name="frm_admin" value="0"> Read-Only
					<INPUT type="radio" name="frm_admin" value="1" checked> Regular User
					<INPUT type="radio" name="frm_admin" value="2"> Administrator';
				break;
				case "2":
					print '<INPUT type="radio" name="frm_admin" value="0" > Read-Only
					<INPUT type="radio" name="frm_admin" value="1"> Regular User
					<INPUT type="radio" name="frm_admin" value="2" checked> Administrator';
				break;
			}
			print '</TD></TR>';
			print '<TR>
				<TD CLASS="td_label">Info:</TD>
				<TD><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$row[info].'" NAME="frm_info"></TD>
			</TR>';
			print '<TR>
				<TD CLASS="td_label">Remove User:</TD>
				<TD ALIGN="left"><INPUT TYPE="checkbox" VALUE="yes" NAME="frm_remove"> (check to remove user)</TD>
			</TR>';
			print '<TR>
				<TD><INPUT TYPE="submit" VALUE="Apply"></FORM></TD>';
			print'<TD align="right"> <FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel"></TD>';
			print '</TR>	</FORM></TABLE>';
		break;
		default :
			if ($debug) print '<br> case default';
		break;
	}
}


/***********************************************************************
*** function which_fields() ********************************************
*** Which fields do we need to display, and in what order **************
***********************************************************************/
function which_fields($table)
{
	global $debug;
	//For Debugging
	global $table_fields;
	$table_fields= array();
	// Retrieve the field-numbers of $table to be displayed in list
	global $link_next ;
	global $mysql_db_next;
	//$query_here = "SELECT `value`, `orden` FROM `options` ";
	$query_here = "SELECT `value`FROM `options` ";
	$query_here = $query_here . " WHERE 1 AND `table_data` = '$table' AND `field` = 'in_listing' ";
	$query_here = $query_here . " ORDER BY `orden` ASC  LIMIT 0 , 100";
	if ($debug)
	{
		print '<p> functions01.inc::which_fields() with $table= '.$table ;
		print '<br> functions01.inc::which_fields() with $query_here= '.$query_here ;
		print '<br> functions01.inc::which_fields() with sizeof($table_fields)= '.sizeof($table_fields) ;
	}
	$result_here = mysql_query($query_here) or
			do_error('functions01.inc::which_fields()::mysql_query()', 'mysql query failed with query' . $query_here, mysql_error());
	//$row_here = mysql_fetch_array($result_here, MYSQL_NUM);
	$i=0;
	while ($row_here = mysql_fetch_array($result_here, MYSQL_BOTH))
	{
		 $table_fields[$i] = $row_here[0]; // row[0] contains `orden`
		 if ($debug) {print '<br> table_fields $1='.$i .' and ' .$table_fields[$i] ;}
		 $i++;
	}
	if ($debug)
	{
		print '<br> functions01.inc::which_fields() with sizeof($table_fields)= '.sizeof($table_fields) ;
	}
	return 0;
	//return $table_fields;
}


/***********************************************************************
*** function do_something_general() ************************************
*** Work needs to be done  *********************************************
***********************************************************************/
function do_something_general()
{
	print '<p>Nothing here -yet-';
	powered("1");
}

?>
