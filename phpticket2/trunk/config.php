<?php
/**************************************
*** File: config.php   ****************
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

	require_once('conf.inc');
	require_once('functions01.inc');
	do_login('config.php');

	/* get help for settings */
	function get_setting_help($setting)
	{
		switch($setting)
		{
			case 'version': 				return 'phpticket (ticket2) version number'; break;
			case 'host': 					return 'hostname where phpticket (ticket2) will run'; break;
			case 'framesize': 				return 'size of the top frame in pixels'; break;
			case 'frameborder': 			return 'size of frameborder'; break;
			// case 'allow_notify': 			return 'allow/deny notification of ticket updates'; break;
			case 'login_banner': 			return 'message shown at login screen'; break;
			case 'abbreviate_description': 	return 'abbreviates descriptions at this number when listing elements, 0 to turn off'; break;
			// case 'abbreviate_at': 			return 'abbreviate at this many letters'; break;
			case 'validate_email': 			return 'simple email validation check'; break;
			case 'abbreviate_affected': 	return 'abbreviates \'affected\' string at this number when listing elements, 0 to turn off'; break;
			// case 'allow_custom_tags': 		return 'enable/disable use of custom tags for rowbreak, italics etc.'; break;
			case 'allow_anonymous_login': 	return 'enable/disable anonymous login'; break;
			default: 						return "No help for '$setting'"; break;
		}
	}
	//set_htmlheader("Settings");
	do_title("Settings");
	global $debug;
	$debug = get_variable("debug_value");
	switch ($_GET['mode'])
	{
	case ("profile"): 	//update profile
		if ($_GET['go'] == 'true')
		{
			//check passwords
			if($_POST['frm_passwd'] != '')
			{
				if($_POST['frm_passwd'] != $_POST['frm_passwd_confirm'])
				{
					print 'Passwords doesn\'t match. Please try again.';
					print '	<BR><BR><TABLE BORDER="0">';
					print '<FORM METHOD="POST" ACTION="config.php?mode=profile&go=true">';
					print '<TR><TD CLASS="td_label">New Password:</TD>
						<TD colspan="2"><INPUT MAXLENGTH="30" SIZE="40" TYPE="password" NAME="frm_passwd"  tabindex="1"></TD></TR>';
					print '<TR><TD CLASS="td_label">Confirm Password:</TD>
						<TD colspan="2"><INPUT MAXLENGTH="40" SIZE="40" TYPE="password" NAME="frm_passwd_confirm"  tabindex="1"></TD></TR>';
					print '<TR><TD CLASS="td_label">Email:</TD>
						<TD colspan="2"><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$_POST['frm_email'].'" NAME="frm_email" tabindex="1"></TD></TR>';
					print '<TR><TD CLASS="td_label">Info:</TD>
						<TD colspan="2"><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$_POST['frm_info'].'" NAME="frm_info" tabindex="1"></TD></TR>';
					print '<INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$_POST['frm_id'].'">';
					print '<TR><TD></TD><TD><INPUT TYPE="submit" VALUE="Apply"></FORM></TD>';
					print '<td align="right"><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM></td>';
					print '</TR></TABLE>';
					powered(); // Print "Powered by" and end the HTML page
					exit();
				}
				else
					$set_passwd = 1;
			}
			else if($_POST['frm_passwd_confirm'] != '')
				print '<FONT CLASS="warn">You need to fill in both password fields. Password is not updated.</FONT><BR>';
			if(!$set_passwd)  $query = "UPDATE user SET info='$_POST[frm_info]',email='$_POST[frm_email]' WHERE id='$_POST[frm_id]'";
			else $query = "UPDATE user SET passwd=PASSWORD('$_POST[frm_passwd]'),info='$_POST[frm_info]',email='$_POST[frm_email]'
					WHERE id='$_POST[frm_id]'";
			$result = mysql_query($query) or do_error('config.php::profile(update)', 'mysql_query() failed', mysql_error());
			print '<B>Your profile has been updated.</B><BR><BR>';
		}
		else
		{
			$id = get_id_from_user($_SESSION['ticket_username']);
			if ($id < 0 OR check_for_rows("SELECT id FROM user WHERE id='$id'") == 0)
			{
				print "Invalid user id '$id'.";
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
			$query	= "SELECT * FROM user WHERE id='$id'";
			$result	= mysql_query($query) or do_error('config.php::profile(get)', 'mysql_query() failed', mysql_error());
			$row	= mysql_fetch_array($result);
			print'<FONT CLASS="header">Edit My Information</FONT><BR><BR><TABLE><TR><TD>';

			print'<TABLE BORDER="0">
				<FORM METHOD="POST" ACTION="config.php?mode=profile&go=true">
				<INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$id.'">';
			print '<TR><TD CLASS="td_label">New Password:</TD>
				<TD colspan="2"><INPUT MAXLENGTH="30" SIZE="40" TYPE="password" NAME="frm_passwd"></TD></TR>';
			print '<TR><TD CLASS="td_label">Confirm Password:</TD>
				<TD colspan="2"><INPUT MAXLENGTH="40" SIZE="40" TYPE="password" NAME="frm_passwd_confirm"></TD></TR>';
			print '<TR><TD CLASS="td_label">Email:</TD>
				<TD colspan="2"><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$row[email].'" NAME="frm_email"></TD></TR>';
			print'<TR><TD CLASS="td_label">Info:</TD>
				<TD colspan="2"><INPUT SIZE="40" MAXLENGTH="80" TYPE="text" VALUE="'.$row[info].'" NAME="frm_info"></TD></TR>';
			print '<INPUT TYPE="hidden" NAME="frm_id" VALUE="'.$id.'">
				<TR><TD></TD><TD><INPUT TYPE="submit" VALUE="Apply"></FORM></TD>';
			print '<td align="right"><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM></td>';
			print '</tr></TABLE>';
			print '</TD><td>';
			print '<TABLE BORDER="0">';
			print '<tr><td>';
			print '<P CLASS="severity_medium">
				* To Change your pasword, fill in the fields <br>"New Password" and "Confirm Password"<br><br>
				* To leave your password "as is", <br>leave both password fields empty<br><br></P>';
			print '</td></tr>';
			print "</TABLE>\n";
			print '</td></tr>';
			print "</TABLE>\n";
			powered(); // Print "Powered by" and end the HTML page
			exit();
		}
	break;
	case "optimize":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			optimize_db();
			print '<FONT CLASS="header">Database optimized.</FONT><BR><BR>';
		}
	break;
	case "debug":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			print '<FONT CLASS="header">Debug Output:</FONT><BR>';
			debug_output();
			print '<BR><BR>';
		}

	break;
	case "reset":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			if ($_GET['auth'] != 'true')
			{
				print '<FONT CLASS="header">Reset Database</FONT>
					<BR>This operation requires confirmation by entering "yes" into this box.<BR>
					<FONT CLASS="warn"><BR>Warning! This deletes all previous tickets, actions, users, resets
					<BR> settings and creates a default admin user.</FONT><BR><BR>
					<TABLE BORDER="0"><FORM METHOD="POST" ACTION="config.php?mode=reset&auth=true">
					<TR><TD CLASS="td_label">Reset tickets/actions:</TD>
						<TD  colspan="2" ALIGN="right"><INPUT TYPE="checkbox" VALUE="1" NAME="frm_ticket"></TD></TR>
					<TR><TD CLASS="td_label">Reset users:</TD>
						<TD colspan="2" ALIGN="right"><INPUT TYPE="checkbox" VALUE="1" NAME="frm_user"></TD></TR>
					<TR><TD CLASS="td_label">Reset settings:</TD>
						<TD colspan="2" ALIGN="right"><INPUT TYPE="checkbox" VALUE="1" NAME="frm_settings"></TD></TR>
					<TR><TD CLASS="td_label">Really reset database? &nbsp;&nbsp;</TD>
						<TD colspan="2" ><INPUT MAXLENGTH="20" SIZE="40" TYPE="text" NAME="frm_confirm"></TD></TR>
					<TR><TD></TD><TD><INPUT TYPE="submit" VALUE="Apply"></FORM></TD>';
					print '<td align="right"><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM></td>';
				print '</TR></TABLE>';
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
			else
			{
				if ($_POST['frm_confirm'] == 'yes') reset_db($_POST['frm_user'], $_POST['frm_ticket'], $_POST['frm_settings']);
				else print '<FONT CLASS="warn">Not authorized or confirmation failed.</FONT><BR><BR>';
			}
		}
	break;
	case "settings":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			if($_GET['go'] == 'true')
			{
				for ($i = 0; $i < $_POST["frm_values"]; $i++)
				{
					$query = "UPDATE settings SET value='".$_POST[frm_setting_value][$i]."' WHERE id='".$_POST[frm_setting_id][$i]."'";
					if ($debug) print '<p> $query: ' . $query ;
					$result = mysql_query($query) or do_error('config.php::save_setting #$i', 'mysql_query() failed', mysql_error());
				}
				print '<FONT CLASS="header">Settings saved.</FONT><BR><BR>';
			}
			else
			{
				print '<FONT CLASS="header">System Settings</FONT><BR><BR><TABLE BORDER="0">
					<FORM METHOD="POST" ACTION="config.php?mode=settings&go=true">';
				$counter = 0;
				$query = 'SELECT * FROM settings ORDER BY name';
				$result = mysql_query($query) or do_error('config.php::list_settings', 'mysql_query() failed', mysql_error());
				while($row = mysql_fetch_array($result))
				{
					print '<TR><TD CLASS="td_label"><A HREF="#" TITLE="'.get_setting_help($row[name]).'">'.$row[name].'</A>: &nbsp;</TD>'. " \n ";
					print '<TD colspan="2" ><INPUT MAXLENGTH="255" SIZE="40" TYPE="text" VALUE="'.$row[value].'" NAME="frm_setting_value[]">';
					print '<INPUT TYPE="hidden" NAME="frm_setting_id[]" VALUE="'.$row[id]."\"></TD></TR> \n ";
					$counter++;
				}
				print '<TR><TD></TD><TD ><INPUT TYPE="hidden" NAME="frm_values" VALUE="'.$counter.'">'." \n ";
				print '<INPUT TYPE="submit" VALUE="Apply"></FORM></TD>'." \n ";
				print '<td align="right"><FORM METHOD="post" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM></td></tr>'." \n ";
				print '</TABLE>'." \n ";
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
		}
	break;
	case "id":
		if ($_SESSION['ticket_user_is_admin']<2) print "<p><FONT CLASS=\"warn\">Not authorized.</FONT><BR><BR>";
		else
		{
			$id = $_GET['id'];
			if ($id < 0 OR check_for_rows("SELECT id FROM user WHERE id='$id'") == 0)
			{
				print "<p><FONT CLASS=\"warn\">Invalid user id '$id'.</FONT><BR><BR>";
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
			$query	= "SELECT * FROM user WHERE id='$id'";
			$result	= mysql_query($query) or do_error('config.php::edit_user(edit)', 'mysql_query() failed', mysql_error());
			$row	= mysql_fetch_array($result);
			user_config(3, $row); // This is the #3 user config screen
			powered(); // Print "Powered by" and end the HTML page
			exit();
		}
	break;
	case "edit":
		if ($debug) print "<p>Mode = edit<br>";
		if ($_POST['frm_remove'] == 'yes')
		{
			if ($debug) print "<p>frm_remove = yes<br>";
			//delete user
			$query = "DELETE FROM user WHERE id='$_POST[frm_id]'";
			$result = mysql_query($query) or do_error('config.php::edit_user(mode=edit::remove)', 'mysql_query() failed', mysql_error());
			print '<p><B>User has been deleted from database.</B></P>';
			print '<p><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Go Back"></FORM><p>';
		}
		else
		{
			if ($debug) print "<p>frm_remove is *not* 'yes'<br>";
			//fix admin value
			if ($_POST['frm_admin'] != 1) $_POST['frm_admin'] = 0;
			if ($_POST['frm_passwd'] == '')
			{
				$query = "UPDATE user SET user='$_POST[frm_user]',info='$_POST[frm_info]',admin='$_POST[frm_admin]'
					WHERE id='$_POST[frm_id]'";
				if ($debug) print "<p>Password is empty and \$query = $query<br>";
			}
			else
			{
				if($_POST['frm_passwd'] != $_POST['frm_passwd_confirm'])
				{
					print 'Passwords doesn\'t match. Try again.<BR>';
					print '<p><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Go Back"></FORM><p>';
					powered(); // Print "Powered by" and end the HTML page
					exit();
				}
				$query = "UPDATE user SET user='$_POST[frm_user]',passwd=PASSWORD('$_POST[frm_passwd]'),
					info='$_POST[frm_info]',admin='$_POST[frm_admin]' WHERE id='$_POST[frm_id]'";
				if ($debug) print "<p>Password is full and \$query = $query<br>";
			}
			$result = mysql_query($query) or do_error('config.php::edit_user(mode=edit::update)', 'mysql_query() failed', mysql_error());
			print '<B>User has been updated.</B><BR><BR>';
		}
	break;
	case "manager":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			if ($debug) print '<p> Checked if user is admin. User is admin';
			if($_GET['go'] == 'true')
			{
				if ($debug) print '<p> $go = true';
				$the_query = $_POST['frm_query'];
				manager_options("show",$the_query); // Show result of applying manager options
				//print '<p><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Go Back"></FORM><p>';
			} else {
				if ($debug) print '<p> $go = false ' .  ' <p>Going to user_config(1) $go=false ';
				manager_options("start"); // Draw select manager options start screen

				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
		}
	break;
	case "adduser":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			if ($debug) print '<p> Checked if user is admin. User is admin';
			if($_GET['go'] == 'true')
			{
				if ($debug) print '<p> $go = true';
				if (check_for_rows("SELECT user FROM user WHERE user='$_POST[frm_user]'"))
				{
					print "<FONT CLASS=\"warn\">User '$_POST[frm_user]' already exists in database. Try again.</FONT><BR>";
					if ($debug) print '<p>Going to user_config(0)';
					user_config(0); // This is the #2 user config screen, It's the same as case #0
					powered(); // Print "Powered by" and end the HTML page
					exit();
				}
				if($_POST['frm_passwd'] == $_POST['frm_passwd_confirm'])
				{
					if ($debug) print '<p> Passwords match';
					if ($_POST['frm_admin'] != 1) $_POST['frm_admin'] = 0;// Set admin value
					$query = "INSERT INTO user (user,passwd,info,admin)
						VALUES('$_POST[frm_user]',PASSWORD('$_POST[frm_passwd]'),'$_POST[frm_info]','$_POST[frm_admin]')";
					if ($debug) print '<p> $query='.$query;
					$result = mysql_query($query) or do_error('config.php::add_user()', 'mysql_query() failed', mysql_error());
					print "<P><B>User '$_POST[frm_user]' has been added.</B><BR><BR>";
				}
				else
				{
					print 'Passwords doesn\'t match. Please try again.<BR>';
					if ($debug) print '<p>Going to user_config(0) after passwords don\'t match';
					user_config(0); // This is the #0 user config screen
					powered(); // Print "Powered by" and end the HTML page
					exit();
				}
			}
			else
			{
				if ($debug) print '<p> $go = false ' .  ' <p>Going to user_config(1) $go=false ';
				user_config(1);	// This is the #1 user config screen
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
		}
	break;
	case "addloc":
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			if ($debug) print '<p> Checked if user is admin. User is admin';
			if($_GET['go'] == 'true')
			{
				if ($debug) print '<p> $go = true';
				if (check_for_rows("SELECT `field` FROM `options` WHERE 1
							AND `table_data` LIKE 'location' AND `field`='$_POST[frm_loc]'"))
				{
					print "<FONT CLASS=\"warn\">Location '$_POST[frm_loc]' already exists in database. Try again.</FONT><BR>";
					if ($debug) print '<p>Going to location_config(0)';
					location_config(0);
					powered(); // Print "Powered by" and end the HTML page
					exit();
				}
				if(trim($_POST['frm_loc']) != "")
				{
					if ($debug) print '<p> Location not empty';

					$query = "INSERT INTO `options` (`orden`, `table_data`, `field`, `value`)
						VALUES('$_POST[frm_value]','location','$_POST[frm_loc]','$_POST[frm_value]')";
					if ($debug) print '<p> $query="'.$query.'"';
					$result = mysql_query($query) or do_error('config.php::add_user()', 'mysql_query() failed', mysql_error());
					print "<P><B>User '$_POST[frm_user]' has been added.</B><BR><BR>";
				}
				else
				{
					print '<p>Location is empty. Please try again.<BR>';
					if ($debug) print '<p>Going to location_config(0) after "location is empy';
					location_config(0);
					powered(); // Print "Powered by" and end the HTML page
					exit();
				}
			}
			else
			{
				if ($debug) print '<p> $go = false ' .  ' <p>Going to location_config(1) $go=false ';
				location_config(1);	// This is the #1 user config screen
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
		}
	break;
	case "debugmode":
		if ($_SESSION['ticket_user_is_admin']<2) print "<p><FONT CLASS=\"warn\">Not authorized.</FONT><BR><BR>";
		else
		{
			if($_GET['go'] != 'true')
			{
				$debugmode = get_variable("debug_value");
				if ($debugmode == "1")
				{
					$checked_debug="CHECKED";
					$unchecked_debug="";
					print 'debugmode = "'. $debugmode .'"';
				} else
				{
					$checked_debug="";
					$unchecked_debug="CHECKED";
				}
				print '<BR><TABLE BORDER="0">
				<FORM METHOD="POST" ACTION="config.php?mode=debugmode&go=true">';
				print '<TR><TD CLASS="td_label">Activate Debug Mode:</TD>
					<td colspan="2"><INPUT TYPE="radio" tabindex="1" NAME="frm_debug"  SIZE="30" VALUE="1"  '.$checked_debug.'></TD></TR>';
				print '<TR><TD CLASS="td_label">De-activate Debug Mode:</TD>
					<td colspan="2"><INPUT TYPE="radio" tabindex="1" NAME="frm_debug"  SIZE="30" VALUE="0"  '.$unchecked_debug.'></TD></TR>';
				print '<TR><TD></TD><TD><INPUT TYPE="submit" VALUE="Change Debug Value"></FORM></TD>';
				print '<td align="right"><FORM METHOD="POST" ACTION="config.php"><INPUT TYPE="submit" VALUE="Cancel / Go Back"></FORM></td>';
				print '</tr></TABLE>';
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
			else
			{
				$debug=$_POST["frm_debug"] ;
				//$query 	= "SELECT name,value FROM settings WHERE name='debug_value'";
				$query = "UPDATE settings SET value='".$debug."' WHERE name='debug_value'";
				$result = mysql_query($query) or do_error("config.php::change debug value::mysql_query()", 'mysql query failed', mysql_error());
				print '<FONT CLASS="header">Debugging Value saved.</FONT><BR><BR>';
				powered(); // Print "Powered by" and end the HTML page
				exit();
			}
		}
	break;
	case "optionstable" :
	if ($_SESSION['ticket_user_is_admin']<2) { print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>'; }
		else
		{
			switch ($_GET["go"])
			{
				case "":
				//edit_input_options("nogo");
				edit_options("nogo");
				break;
				case "edit":
				//edit_input_options("edit");
				edit_options("edit");
				break;
				case "update":
				//edit_input_options("update");
				edit_options("update");
				break;
			}
			//print '<HR>';
			print '<table cellpadding=8><tr><td>';
			print '<FORM METHOD="POST" ACTION="config.php">';
			print '<INPUT TYPE="submit" VALUE="Back to &nbsp; <Settings>">';
			print '</form>';
			if ($_GET["go"])
			{
				print '</td><td>';
				print '<FORM METHOD="POST" ACTION="config.php?mode=optionstable">';
				print '<INPUT TYPE="submit" VALUE="Cancel / Go Back">';
				print '</form>';
			}
			print '</td></tr></table>';
			powered(); // Print "Powered by" and end the HTML page
			exit();
		}
	break;
	case "OLDoptionstable" :
		if ($_SESSION['ticket_user_is_admin']<2) print '<FONT CLASS="warn">Not authorized.</FONT><BR><BR>';
		else
		{
			if($_GET['go'] == 'true')
			{
				for ($i = 0; $i < $_POST["frm_field"]; $i++)
				{
					$query = "UPDATE settings SET field='".$_POST[frm_field][$i]."' WHERE id='".$_POST[frm_setting_id][$i]."'";
					$result = mysql_query($query) or do_error('config.php::save_optionstable #$i', 'mysql_query() failed', mysql_error());
				}
				for ($i = 0; $i < $_POST["frm_delete"]; $i++)
				{
					$query = "UPDATE settings SET value='".$_POST[frm_value][$i]."' WHERE id='".$_POST[frm_setting_id][$i]."'";
					$result = mysql_query($query) or do_error('config.php::save_optionstable #$i', 'mysql_query() failed', mysql_error());
				}
				for ($i = 0; $i < $_POST["frm_field"]; $i++)
				{
					$query = "UPDATE settings SET field='".$_POST[frm_field][$i]."' WHERE id='".$_POST[frm_setting_id][$i]."'";
					$result = mysql_query($query) or do_error('config.php::save_optionstable #$i', 'mysql_query() failed', mysql_error());
				}
				print '<FONT CLASS="header">Settings saved.</FONT><BR><BR>';
			}
			else
			{
				print '<FONT CLASS="header">Input Options</FONT><BR><BR><TABLE BORDER="0">
					<FORM METHOD="POST" ACTION="config.php?settings=true&go=true">';
				$counter = 0;
				$query = 'SELECT * FROM options ORDER BY table_data,field,value ASC';
				$result = mysql_query($query) or do_error('config.php::list_optionstable', 'mysql_query() failed', mysql_error());
				// Print table headers
				$header_names = array ("ID", "Table", "Order", "Field", "Value", "Delete?");
				print '<TR>';
				foreach($header_names as $this_header) {
					print '<TD CLASS="td_link">'.$this_header.'</TD>';
				}
				print "</TR>\n";
				$tblcolor = "td_grey";
				while($row = mysql_fetch_array($result))
				{
					print '<TR><TD CLASS="'.$tblcolor.'">'.$row[id].': &nbsp;</TD>';
					print '<TD CLASS="'.$tblcolor.'">'.$row[table_data].'</TD>';
					print '<TD CLASS="'.$tblcolor.'"><INPUT MAXLENGTH="3" SIZE="5" TYPE="text" VALUE="'.$row[orden].'" NAME="frm_orden[]\></TD>';
					print '<TD CLASS="'.$tblcolor.'"><INPUT MAXLENGTH="35" SIZE="15" TYPE="text" VALUE="'.$row[field].'" NAME="frm_field[]"></TD>';
					print '<TD CLASS="'.$tblcolor.'"><INPUT MAXLENGTH="255" SIZE="50" TYPE="text" VALUE="'.$row[value].'" NAME="frm_value[]">';
					print '<INPUT TYPE="hidden" NAME="frm_setting_id[]" VALUE="'. $row[id] . '"></TD>';
					print '<TD CLASS="'.$tblcolor.'" ALIGN="left"><INPUT TYPE="checkbox" VALUE="1" NAME="frm_delete[]">Yes, delete me</TD>';
					print "</TR>\n";
					// Ternary operator for bi-color rows, log-paper style.
					$tblcolor == "td_grey" ? 	$tblcolor = "td_darkgrey" : $tblcolor = "td_grey";
					$counter++;
				}
				print '<TR><TD COLSPAN="2"> </TD><TD ALIGN="RIGHT"><INPUT TYPE="hidden" NAME="frm_values" VALUE="'.$counter.'">
					<INPUT TYPE="submit" VALUE="Apply"></TD><TD> </TD></TR></FORM></TABLE>';
					powered(); // Print "Powered by" and end the HTML page
				exit();
			}
		}
	break;
	}
	if ($_SESSION['ticket_user_is_admin']>1)
	{
		print '<table cellpadding="10" >';//colpadding="20">';
		print ' <tr><td><A HREF="config.php?mode=adduser">Add New User</A></td><td>Add users to this application</td></tr>';
		print ' <tr><td><A HREF="config.php?mode=manager">Manager Reports</A></td><td>Build and present Manager Reports</td></tr>';
		print ' <tr><td><A HREF="config.php?mode=settings">System Settings</td><td>Manage systemwide settings</td></tr>';
		//print ' <tr><td><A HREF="config.php?mode=reset">Reset Database</A></td><td>empties database</td></tr>';
		//print ' <tr><td><A HREF="config.php?mode=optimize">Optimize Database</A></td><td>optimizes the database</td></tr>';
		print ' <tr><td><A HREF="config.php?mode=addloc">Locations Management</A></td><td>adds a new Location option the drop-down field</td></tr>';
		print ' <tr><td><A HREF="config.php?mode=optionstable">Input Options</A></td><td>Mange input options and parameters</td></tr>';
		print ' <tr><td><A HREF="config.php?mode=debugmode">Turn Debugging on/off</A></td><td>when on, display all debugging information</td></tr>';
		print ' <tr><td><A HREF="config.php?mode=debug">Output Debug Info</A></td><td>displays technical data and information</td></tr>';
		print '</table>';
		print '<br><hr><br>';
	}
	print '<FORM METHOD="POST" ACTION="config.php?mode=profile">';
	//print '<LI><A HREF="config.php?mode=profile">Edit My Information</A></LI><BR><BR>';
	print '<INPUT TYPE="submit" VALUE="Edit My Information">';
	print '</form>';
	if ($_SESSION['ticket_user_is_admin']>1)
	{
		show_stats();
		list_users();
	}
	powered(); // Print "Powered by" and end the HTML page
	//print '</BODY></HTML>';
?>
