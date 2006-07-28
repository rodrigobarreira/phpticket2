<?php
/**************************************
*** File: functions02.inc  ********
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/



/*************************************************************
*** function add_element_buttons($id, $page, $title) *********
*** add Buttons to $element info *****************************
*************************************************************/
function add_element_buttons($id)
{
	global $debug;
	if ($debug)
	{
		print 'functions02.inc::add_element_buttons()';
		print '<br>$id = "'.$id.'"';
		print '<br>$_SESSION[ticket_user_is_admin] = "'.$_SESSION['ticket_user_is_admin'].'"';
		print '<br>$GLOBALS[ELEMENT] = "'.$GLOBALS['ELEMENT'].'"';
		print '<br>$GLOBALS[MAIN_PAGE] =  "'.$GLOBALS['MAIN_PAGE'].'"';
		print '<br>$_GET[action] = "'.$_GET['action'].'"';
		print '<br>$_GET[closed] = "'.$_GET['closed'].'"';
	}
	// Remove trailing 's' from $title.		
	if (trim($id) == '')
	{
		do_error('functions02.inc::add_element_buttons()', 'missing parameter "id"', 'A mandatory paramater was not provided');
		return;
	}
	if ($_SESSION['ticket_user_is_admin'] > 0) 
	{
		$name = $GLOBALS['ELEMENT'];
		$url = $GLOBALS['MAIN_PAGE'].'.php';
		print '<table cellpadding="10" > <tr>';
		if  ($debug)
		{
			print '<p>functions02.inc::add_element_buttons()';
			print '<br>$_GET[action] = "'.$_GET['action'].'"';
			print '<br>$_GET[closed]  = "'.$_GET['closed'] .'"';
		}
	
		if ( (trim($_GET['action']) != "close") &&  ($_GET['closed'] != "true") )
		{
			print '<td><FORM METHOD="post" ACTION="'.$url.'?id='.$id.'&action=close">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Close '.$name.'"></FORM> </td> ';		print "\n";
		}
	
		if (trim($_GET['action']) != "" && $_GET['action'] != "ownupdat" && $_GET['action'] != "own" )
		{
			print '<td><FORM METHOD="post" ACTION="'.$url.'?id='.$id.'">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Return to '.$name.' #'.$id.'"></FORM> </td>  ';		print "\n";
		} else {
			print '<td><FORM METHOD="post" ACTION="'.$url.'?id='.$id.'&action=edit">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Edit '.$name.'\'s Data"></FORM> </td>  ';		print "\n";
		}
		print '<td><FORM METHOD="post" ACTION="'.$url.'?id='.$id.'&action=delet">'; print "\n";
		print '<INPUT TYPE="submit" VALUE="Delete '.$name.'"></FORM> </td> ';		print "\n";
		if (trim($_GET['closed']) != "true")
		{
			/*****
			print '<td><FORM METHOD="post" ACTION="'.$url.'">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Back to Main Page"></FORM> </td>';		print "\n";
			******************/
			print '<td>';
			go_back($url);
			print '</td>';
		} else
		{
			print '<td><FORM METHOD="post" ACTION="'.$url.'?showclosed=true">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Back to Closed Tickets"></FORM> </td>';		print "\n";
		}
		print " </tr></table> \n";
	}
}


/*************************************************************
*** function build_add_form($id,$table,$element) *************
*** Builds a form on-the-fly for adding the retrieved data **
*************************************************************/
function build_add_form($id)
{
	global $debug;
	global $portal_user;
	$table =$GLOBALS['MAIN_TABLE'];
	//For Debugging
	if ($debug)
	{
		print '<p> functions02.inc::build_add_form()';
		print '<br>with $id= '.$id;
		print '<br>with $table= '.$GLOBALS['MAIN_TABLE'];
		print '<br>with $portal_user = ' . $portal_user ;
	}
	// Retrieve the field-names of $table from the database $mysql_db_next
	global $link_next ;
	global $mysql_db_next;
	$fields = mysql_list_fields($mysql_db_next, $table, $link_next);
	$columns = mysql_num_fields($fields);
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $table); // 0 => short. !0 => long
	// Build the SQL query
	$query = "SELECT * FROM $table WHERE 1 LIMIT 1";   // ID='$id'";
	if ($debug)
	{
		print '<p> functions02.inc::build_add_form($id,$table) with<br> $query= '. $query .'<br><br>' ;}
	$result = mysql_query($query) or
			do_error('functions02.inc::build_add_form()::mysql_query()', 'mysql query failed with query ' . $query, mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0" bgcolor="white">';		print "\n";
	$index = 0; // Index for moving around with TAB key.
	// Make it easy to change order of appearance of input fields. Or to make a field not show up at all.
	//$fields_to_show = array(1,2,3,4,5,6,7,8,9,10); // Numbers of Fields to be shown, in order to be shown
	$fields_to_show = array(5,6,2,8,9,4,10); // Numbers of Fields to be shown, in order to be shown
	// $i=0 is "id", as 'id' is an **auto** field, skip it & print rest of fields
	$tab_value = 1;
	while ($field_number = each ($fields_to_show))
	{
		$i = $field_number[1];
		$index++;
		switch ($i)
		{
		case (1):
		case (2):
		case (3): // Dates
		//generate_date_dropdown('problemstart');
			print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			print '<TD COLSPAN="2">';
			$tab_value = generate_date_dropdown(strtolower(mysql_field_name($fields, $i)),0,2000,$tab_value);
			print '</TD> ';
			print "</TD> </TR>\n";
		break;
		case (4): // 2-line inputut field. Scope
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  = '" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><TEXTAREA '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print 'COLS="40" ROWS="2" tabindex="'.$index.'" ></TEXTAREA>';
			print "</TD> </TR>\n";
		break;
		case (5): // (40-chars) / Affected
			print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			print '<TD COLSPAN="2"><INPUT TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print ' SIZE="40" MAXLENGTH="40" VALUE="" tabindex="'.$index.'"></TD> ';  // empty value
			print "</TD> </TR>\n";
		break;
		case (6): // 4-line inputut field. Description
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  = '" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><TEXTAREA '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print 'COLS="40" ROWS="4" tabindex="'.$index.'" ></TEXTAREA>';
			print "</TD> </TR>\n";
		break;
		case (8): // We need to retrieve the options for 'owner' drop-down field and select the one from the DB
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  = '" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'">';
			global $portal_user;
			if ($GLOBALS['DEFAULT_UNASSIGNED'])	{ get_option_owner("1" , $index); }
			else { get_option_owner(get_id_from_user($portal_user) , $index); }
			print '</SELECT>';
			if ($debug) { print " \n <p> Portal User is '$portal_user' \n ";}
			print "</TD> </TR>\n";
		break;
		case (9): // We need to retrieve the options for 'severity' drop-down field and select the one from the DB
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  = '" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'"> ';
			get_option_constant("severity"  , "0", $index);
			print "</TD></TR>\n";
		break;
		case (10): // We need to retrieve the options for 'location' drop-down field and select the one from the DB
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  = '" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'">';
			get_option_constant("location"  , "0", $index);
			print "</TD></TR>\n";
		break;
		}
	}
	// We need to make 'status' field hidden with value = '1'. 'status' is field num. 7
	print '<TR><TD CLASS="td_label">';
	print '<INPUT TYPE="HIDDEN" NAME="frm_'.strtolower(mysql_field_name($fields, 7)).'"  VALUE="'.$GLOBALS['STATUS_OPEN'] .'">';
	print "</TD>\n\n";
	// Create a Hidden field for the "reason for new $element".
	$reason='New '. $GLOBALS['ELEMENT'] .' added to database';
	print '<TD  COLSPAN="2"><INPUT TYPE="hidden" NAME="frm_reason" VALUE="'.$reason.'"></TD>';
	print "</TR></TABLE>\n\n";
	return 0;
}


/*************************************************************
*** function build_edit_form($id,$reason,$page) *******
*** Builds a form on-the-fly for editing the retrieved data **
*************************************************************/
function build_edit_form($id,$reason)
{
	global $debug;
	$page=$GLOBALS['MAIN_PAGE'];
	//For Debugging
	if ($debug)
	{
		print '<p> functions02.inc::build_edit_form()';
		print '<br>with $id = '.$id;
		print '<br>and $table = '.$GLOBALS['MAIN_TABLE'] . " \n ";
	}
	// Retrieve the field-names of $GLOBALS['MAIN_TABLE'] from the database $mysql_db_next
	global $link_next ;
	global $mysql_db_next;
	$fields = mysql_list_fields($mysql_db_next, $GLOBALS['MAIN_TABLE'], $link_next);
	$columns = mysql_num_fields($fields);
	// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
	//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $GLOBALS['MAIN_TABLE']); // 0 => short. !0 => long
	// Build the SQL query
	$query = "SELECT * FROM ".$GLOBALS['MAIN_TABLE']." WHERE ID='$id'";
	if ($debug)
	{
		print '<p> functions02.inc::build_edit_form() with<br> $query= "'. $query .'"' ;
	}
	$result = mysql_query($query) or
		do_error('functions02.inc::build_edit_form()::mysql_query()', 'mysql query failed', mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0"  bgcolor="white">';
	//$i=0 is "id", and it should not change ==> TYPE="hidden"
	print '<TR>';
	print '<TD CLASS="td_label">'.$fieldnames[0].'</TD>';
	// Field name first -> $fieldname . Make sure you avoid spaces at begining/end of variable name as " frm_foo" or "frm_foo "
	print '<TD COLSPAN="2"><INPUT TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, 0)).'" ';
	// Then, the form variable -> frm_strtolower($fieldname),
	print ' SIZE="30" VALUE="'.trim($row[mysql_field_name($fields, 0)]).'">'.$row[mysql_field_name($fields, 0)].'</TD> ';
	// and last, value -> row[$fieldname]
	print '</TR>';	  print "\n";

	if ($debug)
	{
		print '<p> functions02.inc::build_edit_form()';
		print '<br>Ready to sart the table building' ."\n";
	}
	$tab_value = 1;
	for ($i = 1; $i < $columns; $i++)  // Rest of fields
	{
		if ($debug) {
			print '<p>$i = "'. $i .'" of '.$columns;
			print '<br>Description = "' . $fieldnames[$i] .'"';
			print '<br>Value = "'. trim($row[mysql_field_name($fields, $i)]).'"';
		}
		switch ($i)
		{
		case (1):
		case (2):
		case (3): // Dates
			print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print ' SIZE="21" MAXLENGTH="19" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'" ></TD> ';  // empty value
			print "</TD> </TR> \n ";  
		break;
		case (4): // 2-line inputut field. Scope
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><TEXTAREA '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print 'COLS="70" ROWS="2"  >'.trim($row[mysql_field_name($fields, $i)]).'</TEXTAREA>';
			print "</TD> </TR> \n ";  
		break;
		case (5): // (40-chars) / Affected
			print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print ' SIZE="40" MAXLENGTH="40" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'" tabindex="'.$i.'"></TD> ';  // empty value
			print "</TD> </TR> \n ";  
		break;
		case (6): // 5-line inputut field. Description
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><TEXTAREA '.$value.'  NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print 'COLS="70" ROWS="5"  >'.trim($row[mysql_field_name($fields, $i)]).'</TEXTAREA>';
			print "</TD> </TR> \n ";  
		break;
		case (7): // Status: retrieve the options for drop-down field and select the one from the DB
			print '<TR> <TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			print '<TD COLSPAN="2">';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			get_option_constant((mysql_field_name($fields, $i)), trim($row[mysql_field_name($fields, $i)]),$i);
			print '</SELECT> ';
			print "</TD></TR> \n ";	  
		break;
		case (8): // Owner: retrieve the options for drop-down field and select the one from the DB
			print '<TR> <TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			print '<TD COLSPAN="2">';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			//get_option_owner($GLOBALS['MAIN_TABLE'], trim($row[mysql_field_name($fields, $i)]),$i);
			get_option_owner(trim($row[mysql_field_name($fields, $i)]),$i);
			print '</SELECT> ';
			print "</TD></TR> \n ";	  
		break;
		case (9): // Severity: retrieve the options for drop-down field and select the one from the DB
			print '<TR> <TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			print '<TD COLSPAN="2">';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			get_option_constant((mysql_field_name($fields, $i)), trim($row[mysql_field_name($fields, $i)]),$i);
			print '</SELECT> ';
			print "</TD></TR> \n ";	  
		break;
		case (10): // Location: retrieve the options for drop-down field and select the one from the DB
			print '<TR> <TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			print '<TD COLSPAN="2">';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<SELECT '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			get_option_constant((mysql_field_name($fields, $i)), trim($row[mysql_field_name($fields, $i)]),$i);
			print '</SELECT> ';
			print "</TD></TR> \n ";	  
		break;
		}
	}
	// Create a Hidden area for the "reason for change".
	$reason="Data has been modified in the database";
	print '<TR> <TD CLASS= td_label > </TD>
			<TD  COLSPAN="2"><INPUT TYPE="hidden" NAME="frm_reason" VALUE="'.$reason.'"></TD>';
	print "</TR></TABLE>\n";
	return 0;
}




/*************************************************************
*** function build_email_element($id) ********************
*** Build Text for email ***********************************
*************************************************************/
function build_email_element($id, $table)
{
	global $debug;
	// Define global variables for data
	global $link_next ;
	global $mysql_db_next;
	global $debug;
	$link_next = mysql_open_next( $mysql_db_next);
	//mysql_open_next("$mysql_db_next", "$mysql_user_next", "$mysql_passwd_next");
	$fields = mysql_list_fields($mysql_db_next, $table, $link_next); // retrieve list of fields
	$columns = mysql_num_fields($fields);
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $table); // 0 => short. !0 => long

	if ($debug)
	{
		print '<p> functions02.inc::build_email__element()';
		print '<br> $id = '.$id . " \n ";
	}
	if ($id)
	{
		$email_text = "Ticket Number: $id \n "; // "<PRE>"; // Adding first line.
	} else {
		$email_text = ""; // "<PRE>"; // Zeroing email text content.
	}
	if ($debug)
	{
		print '<p> functions02.inc::build_email__element()';
		print "<br> \$id is zero \n";
	}
	for ($i=1 ; $i < $columns; $i++)  // Generate the queries with a loop. Avoids TicketElement ID ($i=0)
	{
		$frm_temp_value = "frm_" .strtolower(mysql_field_name($fields, $i)); // Build  field values' list
		switch ($i) //Build field Values in special way: dates
		{
			case (1): // Ticket date: t_date
				$temp_text =  "'".date("Y-m-d H:i:s") ."',"     ;	//...  with no extra spaces
				//$i =3;
			break;
			case (7):
				$temp_text = "'". get_single_option("status", $_POST[$frm_temp_value])."'";
			break;
			case (8):
				$temp_text = "'".get_owner($_POST[$frm_temp_value])."'";
			break;
			case (9):
				$temp_text = "'". get_single_option("severity", $_POST[$frm_temp_value])."'";
			break;
			case (10):
				$temp_text = "'". get_single_option("location", $_POST[$frm_temp_value])."'";
			break;
			case (4):
			case (5):
			case (6):
				$temp_text = "'".trim($_POST[$frm_temp_value] )."'"     ;	//...  with no extra spaces
			break;
		}
// Build email text: Entered values
		$email_text = $email_text  . $fieldnames[$i] . ": $temp_text \n";
		if ($i ==1) {$i=3;}
	}
	//$email_text = $email_text  . "</PRE> \n";
	if ($debug)
	{
		print '<p> functions02.inc::build_email__element()';
		print "<br> \$email_text is: \n ";
		print "<br>$email_text \n ";
	}

	return $email_text;

}

/*************************************************************
*** function close_element($id, $action) *********************
*** Close the selected element *******************************
*************************************************************/
function close_element($id, $action, $page="") // remnoved $table
{
	// This fuction is **very specific** of the information in the **table**
	// This means, that it doesn't need to be "generalized".

	// Define the element and page that use this function,
	// This is useful to easily convert this fucntion to other elements
	global $debug;
	$element = "Ticket";
	$page=$GLOBALS['MAIN_PAGE'];
	global $portal_user; // $portal_user is the user working in this session.
	// Build Title Sentence
	//$sentence = (($id == 0) ? (ucfirst($action)."ing ".$element) : (ucfirst($action)."ing ".$element." #$id"));
	$sentence = "Closing ".$element." #$id";
	if ($debug)
	{
		print '<p> functions02.inc::close_element()';
		print '<br> $sentence = '.$sentence ."\n";
	}
	$temp_trailing = ($id != 0 )? (" #$id") : (" "); // Whichen adding, $id is *not* really zero
	do_title(ucfirst($action)."ing ".$element." " . $temp_trailing); // Begin the webpage.
	// Debug mode.
	 if ($debug )
	 {
		 print '<p> functions02.inc::close_element()';
		 print '<br>$id =  <b>'.$id.'</b>';
		 print '$_GET[action] = <b>'.$_GET['action'].'</b>';
		 print '$portal_user = '.$portal_user .'<br><br>'; print "\n";
	 }
	switch($action)  // Which course of action should we take?
	{
		// Adding new $element
		case "close":  // Close element: modify element with only "Status=Closed" and giving a "Reason for Closing"
		if ($debug )
		{
			print '<p> functions02.inc::close_element(edit)';
			print '<br>$id = '.$id;
			print '<br>$_GET[\'action\'] = '.$_GET['action'].'<br>';
		}
		if ($id == '' OR $id <= 0)// OR !check_for_rows("SELECT * FROM $GLOBALS['MAIN_TABLE'] WHERE id='$id'"))
		{
			if ($debug )
			{
				print '<p> functions02.inc::close_element(edit)::Warning ';
				print '<br> $id = ' .$id;
				print '<br> $_GET[action] = '	.$_GET['action'];
				print '<br> $portal_user = '.$portal_user ;
				print '<br><FONT CLASS="warn">Invalid $element ID: "'.$id.'"</FONT><BR>';
			}
		}
		else
		{
			$query = "SELECT * FROM ".$GLOBALS['MAIN_TABLE']." WHERE id='$id'";
			if ($debug )
			{
				print '<p>functions02.inc::close_element(edit/updat)';
				print '<br>$id = ' .$id;
				print '<br>$_GET[\'action\'] = '.$_GET['action'];
				print '<br>$query = "'.$query.'"';
				print '<br>$portal_user = '.$portal_user .'<br><br>';
			}
			$result = mysql_query($query) or
				do_error('functions02.inc::close_element:updat', 'mysql_query() failed', mysql_error());
			$row = mysql_fetch_array($result);
			// Show the current information in editable fields from the browser.
			// form name = 'frm' + '_' + 'lowercase(fieldname)'
			print '<table  bgcolor="white"><tr><td valign="top" width="20%">';	  print "\n";
			add_element_buttons($id, "$element" . "s" );
			print '</td></tr><tr><td valign="top" width="75%">';  print "\n";
			print '<FORM METHOD="post" ACTION="'.$page.'.php?id='.$id.'&action=closeupdat">' . " \n ";
			//build_edit_form($id,$GLOBALS['MAIN_TABLE'],$reason,$page); print "\n";

			/****************************************************************
				Code From build_edit_form()
			****************************************************************/
			global $link_next ;
			global $mysql_db_next;
			$fields = mysql_list_fields($mysql_db_next, $GLOBALS['MAIN_TABLE'], $link_next);
			$columns = mysql_num_fields($fields);

			// Initialize array with obvious wrong values, to detect errors.
			$fieldnames = array("long", "day","at","the", "races");
			//Request fildnames, in long version
			$fieldnames = get_fieldname(1, $GLOBALS['MAIN_TABLE']); // 0 => short. !0 => long

			print '<TABLE BORDER="0"  bgcolor="white">';
			//$i=0 is "id", and it should not change ==> TYPE="hidden"
			print '<TR>';
			print '<TD CLASS="td_label">'.$fieldnames[0].'</TD>';
			// Field name first -> $fieldname . Make sure you avoid spaces at begining/end of variable name as " frm_foo" or "frm_foo "
			print '<TD COLSPAN="2"><INPUT TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, 0)).'" ';
			// Then, the form variable -> frm_strtolower($fieldname),
			print ' SIZE="30" VALUE="'.trim($row[mysql_field_name($fields, 0)]).'">'.$row[mysql_field_name($fields, 0)].'</TD> ';
			// and last, value -> row[$fieldname]
			print '</TR>';	  print "\n";
			$tab_value = 1;
			for ($i = 1; $i < $columns; $i++)  // Rest of fields
			{
				switch ($i)
				{
				case (1):
				case (2): // Dates
					print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' SIZE="21" MAXLENGTH="19" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'" >';
					print trim($row[mysql_field_name($fields, $i)]).'</TD> ';
					print "</TD> </TR>\n";  print "\n";
				break;
				case (3): // Closing date
					print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					$date_value = date("Y-m-d H:i:s");
					print ' SIZE="21" MAXLENGTH="19" VALUE="'.$date_value.'" ></TD> ';  // NOW() value
					print "</TD> </TR>\n";  print "\n";
				break;
				case (4): // 2-line inputut field. Scope
					print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2" WRAP><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' VALUE="'.trim($row[mysql_field_name($fields, $i)]).'"  >'.trim($row[mysql_field_name($fields, $i)]);//.'</TEXTAREA>';
					print "</TD> </TR>\n";  print "\n";
				break;
				case (5): // (40-chars) / Affected
					print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2" WRAP><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' SIZE="40" MAXLENGTH="40" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'" >';
					print trim($row[mysql_field_name($fields, $i)]).'</TD> ';
					print "</TD> </TR>\n";  print "\n";
				break;
				case (6): // 5-line inputut field. Description
					print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' VALUE="'.trim($row[mysql_field_name($fields, $i)]).'">'.trim($row[mysql_field_name($fields, $i)]);//.'</TEXTAREA>';
					print "</TD> </TR>\n";  print "\n";
				break;
				case (7): // Status: retrieve the options for drop-down field and select the one from the DB
					print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print '  VALUE="'.$GLOBALS['STATUS_CLOSED'].'">'. get_single_option("status", $row[$i]);
					print ' (click on "<b>Change to Closed</b>" button to close this ticket) ' ;
					print "</TD> </TR>\n";  print "\n";
				break;
				case (8): // Owner: retrieve the options for drop-down field and select the one from the DB
					print "</TR>\n";	  print "\n";
					print '<TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' SIZE="21" MAXLENGTH="19" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'">';
					print get_owner(trim($row[mysql_field_name($fields, $i)])).'</TD> ';
					print "</TD> </TR>\n";  print "\n";
				break;
				case (9): // Severity: retrieve the options for drop-down field and select the one from the DB
					print '<TR> <TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' SIZE="21" MAXLENGTH="19" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'">';
					print get_single_option("severity", $row[$i]).'</TD> ';
					print '</TD> ';
					print "</TR>\n";	  print "\n";
				break;
				case (10): // Location: retrieve the options for drop-down field and select the one from the DB
					print '<TR> <TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
					$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
					print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
					print ' SIZE="21" MAXLENGTH="19" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'">';
					print get_single_option("location", $row[$i]).'</TD> ';
					print '</TD> ';
					print "</TR>\n";	  print "\n";
				break;
				}
			}
			// Create a Hidden area for the "reason for change".
			print '<TR> <TD CLASS= td_label > </TD>
					<TD  COLSPAN="2"><INPUT TYPE="hidden" NAME="frm_status" VALUE="'.$GLOBALS['STATUS_CLOSED'].'"></TD>';
			print "</TR></TABLE>\n";
			/****************************************************************/
			/****************************************************************/
			/****************************************************************/
			print'<BR><INPUT TYPE="submit" VALUE="Change to Closed"></FORM><br>';		print "\n";
			mysql_shutdown_next();
			print '</td></tr></table>';
		}
		break;

		case "closeupdat": 		// Updating the ticket to "Closed"
		if ($debug )
		{
			print '<p> functions02.inc::close_element(update)';
			print '<br>$id = ' .$id ;
			print '<br>$_GET[action] = ' .$_GET['action'];
			print '<br>$portal_user = '.$portal_user ;
			print '<br>$table = '.$GLOBALS['MAIN_TABLE'].'<br><br>' . "\n";
		}
			// update $element
			if ($id == '' OR $id < 0)// OR !check_for_rows("SELECT * FROM $GLOBALS['MAIN_TABLE'] WHERE id='$id'") )
			{
				if ($debug )
				{
					print '<p>functions02.inc::close_element(update)';
					print '<br> Invalid id. $id = ' .$id ;
					print '<br> $_GET[action] = '.$_GET['action'];
					print '<br> $portal_user = '.$portal_user .'<br><br>';
				}
				print " \n <FONT CLASS=\"warn\">Invalid $element: '$id' </FONT>";   print "\n";
				if (trim($element)=="") { $element = trim($GLOBALS['MAIN_TABLE']) ;}
				show_data($id,$GLOBALS['MAIN_TABLE'],$element."s",$page);
			} else
			{
				// Time to do updates.
				print '<FONT CLASS="severity_medium">'.$element.' has been Closed</FONT><BR><BR>';  print "\n";
				//Update $Element function make_insert_query($GLOBALS['MAIN_TABLE'], $type,$id,$visit=0,$aid=0)
				$query=make_insert_query($GLOBALS['MAIN_TABLE'], "close", $id);
				// On error, exit.
				$result = mysql_query($query) or do_error('functions02.inc::close_element(closeupdat)', 'mysql query failed', mysql_error());
				// show list of $elements
				list_data($GLOBALS['MAIN_TABLE'],"",0,0,$element);
			}
//		}
		break;

		// Deleting
		case "delet":
		if ($debug )  print '<p>functions02.inc::close_element(delete)<br>$_POST[frm_confirm] = ' . $_POST['frm_confirm'] ;
		if ($_POST['frm_confirm'])
		{
			if ( $_POST['frm_reason'] == '')
			{
				print "<P CLASS=\"warn\"> No reason given for removal of $element: '$id'. <BR>Try again.</P>";  print "\n";
				show_data($id,$GLOBALS['MAIN_TABLE'], $element,$page);	  print "\n";
			}else
			{
				if ($debug )
				{
					print '<p>functions02.inc::close_element(delete).';
					print '<br>$id = ' .$id ;
					print  '<br> \$_GET[\'action\'] = ' .$_GET['action'] ;
					print  '<br>$portal_user = '.$portal_user ;
					print  '<br>\$_POST[\'frm_confirm\'] = '.$_POST['frm_confirm'].'<br><br>'."\n";
				}
				// remove $element and make sure its logged into the database
				//transaction_management($id, $GLOBALS['MAIN_TABLE'], $action,$_POST['frm_reason'], $portal_user);  print "\n";
				$result = mysql_query("DELETE FROM ".$GLOBALS['MAIN_TABLE']." WHERE ID='$id'")
					or do_error('functions02.inc::close_element(delete)', 'mysql_query() failed', mysql_error());
				print "<P CLASS=\"header\">$element number '$id' has been removed.</P><BR><BR>";	  print "\n";
				list_data($GLOBALS['MAIN_TABLE'],"",0,0,$element);	  print "\n";
			}
		}
		else
		//confirm deletion
		{
			$query = "SELECT * FROM ".$GLOBALS['MAIN_TABLE']." WHERE id='$id'";
			if ($debug )
			{
				print '<p>functions02.inc::close_element(delet)';
				print '<br>$id = ' .$id ;
				print '<br> \$_GET[\'action\'] = '.$_GET['action'];
				print '<br> $query = '.$query;
				print '<br> $portal_user = '.$portal_user .'<br><br>'."\n";
			}
			$result = mysql_query($query) or
				do_error('functions02.inc::close_element:delet', 'mysql_query() failed', mysql_error());
			$row = mysql_fetch_array($result);

			if ($debug )
			{
				print '<p>functions02.inc::close_element(delete)';
				print '<br>$id = ' .$id;
				print '<br> \$_GET[\'action\'] = '.$_GET['action'];
				print '<br>$portal_user = '.$portal_user;
				print '<br>\$_POST[\'frm_confirm\'] = '.$_POST['frm_confirm'].'<br><br>'."\n";
			}

			print "<FONT CLASS=\"severity_medium\">Confirm deletion of $element #$id ";
			print "<br>\"$row[1], $row[2], $row[3], $row[4]\" </FONT>";
			print "<BR><BR><FORM METHOD=\"post\" ACTION=\"$page.php?id=$id&action=delet&go=1\">\n" . "\n";
			print "<TABLE BORDER=\"0\">";
			print "<TR><TD CLASS=\"td_greylabel\">Delete $element #$id ?&nbsp;</TD>
				<TD><INPUT TYPE=\"checkbox\" NAME=\"frm_confirm\" VALUE=\"1\">&nbsp;&nbsp;Yes, delete this $element</TD>
				</TR>" . "\n";
			$reason = ""; 
			print "<TR>
				<TD CLASS=\"td_greylabel\">Provide a Reason for<br>deleting the $element:</TD>
				<TD><TEXTAREA NAME=\"frm_reason\" COLS=\"40\" ROWS=\"4\">$reason</TEXTAREA></TD></TR>\n" . "\n";
			print "</TABLE><br>". "\n";
			print "<INPUT TYPE=\"Submit\" VALUE=\"Confirm\">";
			print "</FORM><br>" . "\n";
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php"><INPUT TYPE="submit" VALUE="Cancel"></FORM> ' . "\n";
		}
		break;

		// Showing info, so we can edit the data.
		case " ":
			print  "<P CLASS=\"warn\"> No action given. Stop.</p>". "\n";
		break;
	}
}


/*************************************************************
*** function edit_element($id, $action) *************************
*** Edit the selected host ***********************************
*************************************************************/
function edit_element($id, $action,$table)
{
	// This fuction is **very specific** of the information in the **table**
	// This means, that it doesn't need to be "generalized".

	// Define the element and page that use this function,
	// This is useful to easily convert this fucntion to other elements
	global $debug;
	$element = $GLOBALS['ELEMENT'];
	$page = $GLOBALS['MAIN_PAGE'] ;// no ".php" here.
	global $portal_user; // $portal_user is the user working in this session.
	$reason = "";
	// Build Title Sentence
	$sentence = (($id == 0) ? (ucfirst($action)."ing ".$element) : (ucfirst($action)."ing ".$element." #$id"));
	if ($debug)
	{
		print '<p> functions02.inc::edit_element()';
		print '<br> $sentence = '.$sentence ."\n";
	}
	$temp_trailing = ($id != 0 )? (" #$id") : (" "); // Whichen adding, $id is *not* really zero
	do_title(ucfirst($action)."ing ".$element." " . $temp_trailing); // Begin the webpage.
	// Debug mode.
	 if ($debug )
	 {
		 print '<p> functions02.inc::edit_element()';
		 print '<br> $id =  <b>'.$id.'</b>';
		 print '<br> $_GET[action] = <b>'.$_GET['action'].'</b>';
		 print '<br> $portal_user = '.$portal_user  ; print "\n";
	 }
	switch($action)  // Which course of action should we take?
	{
		// Adding new $element
		case "add":
		if ($debug )  print '<p>functions02.inc::edit_element(action=add)<br>$id='.$id .'<br>';

		if ($id != 0 ) // sanity check . We don't want to mess existing records (existing record: id != 0 )
		{
			if ($debug )
			{
				print '<p>functions02.inc::edit_element()::action=add id!=0';
				print '<br>Inside invalid id (it should be zero for adding).';
				print '<br>$id = ' .$id ;
				print '<br>$_GET[action] = '.$_GET['action'];
				print '<br>$portal_user = '.$portal_user. "\n";
			}
			print '<P CLASS="warn">Invalid '.$element.' ID: "'.$id.'" . I cannot add. Exiting.</P>'. " \n ";
			return;
		}
		else
		{
			if ($debug )
			{
				print '<p>functions02.inc::edit_element()::action=add id=0';
				print '<br> $id = ' .$id;
				print '<br> $_GET[action] = '.$_GET['action'];
				print '<br> $portal_user = '.$portal_user . " \n ";
			}
			print '<table border="0" cellpadding="2" cellspacing="2" ><tr><td valign="top" >';	  print "\n";
			print '<FORM METHOD="post" ACTION="'.$page.'.php?id=0&action=updat">';
			build_add_form(0);
			print '<BR><INPUT TYPE="submit" VALUE="Submit Changes"></FORM><br>';		  print "\n";
			print '</td><td valign="top">';
			print '<P CLASS="severity_medium"> * Use TAB key to move around fields</P>';
			print '<P CLASS="severity_medium"> * Please fill in all info</P>';
			print '<P CLASS="severity_medium"> * Please enter Phone Number in 123-456-7890 format<br> </P>';
			print '<P CLASS="severity_medium"> * Please enter dates in YY-MM-DD format<br> </P>';
			print '<P CLASS="severity_medium"> * Review bottom list for quick cut & paste</P>'; print "\n";
			print '</td></tr></table>' . " \n ";
			do_title("List of previous $element"."s");
			list_data($table,"", 0,0,$element);
			mysql_shutdown_next();
		}
		break;

		case "edit":  // Editing
		if ($debug )
		{
			print '<p> functions02.inc::edit_element(edit)';
			print '<br>$id = '.$id;
			print '<br>$_GET[action] = '.$_GET['action']. " \n ";
		}
		if ($id == '' OR $id <= 0)// OR !check_for_rows("SELECT * FROM $table WHERE id='$id'"))
		{
			if ($debug )
			{
				print '<p> functions02.inc::edit_element(edit)::Warning ';
				print '<br> $id = ' .$id;
				print '<br> $_GET[action] = '	.$_GET['action'];
				print '<br> $portal_user = '.$portal_user ;
				print '<br><FONT CLASS="warn">Invalid $element ID: "'.$id.'"</FONT><BR>';
			}
		}
		else
		{
			$query = "SELECT * FROM $table WHERE id='$id'";
			if ($debug )
			{
				print '<p>functions02.inc::edit_element(edit/updat)';
				print '<br>$id = ' .$id;
				print '<br>$_GET[action] = ' . $_GET['action'];
				print '<br>$query = '.$query;
				print '<br>$portal_user = '.$portal_user . " \n ";
			}
			$result = mysql_query($query) or
				do_error('functions02.inc::edit_element:updat', 'mysql_query() failed', mysql_error());
			$row = mysql_fetch_array($result);
			// Show the current information in editable fields from the browser.
			// form name = 'frm' + '_' + 'lowercase(fieldname)'
			print '<table  bgcolor="white"><tr><td valign="top" width="20%">' .  " \n ";
			add_element_buttons($id, $element );
			print '</td></tr><tr><td valign="top" width="75%">';  print "\n";
			print '<FORM METHOD="post" ACTION="'.$page.'.php?id='.$id.'&action=updat">' . " \n ";
			build_edit_form($id,$reason); print "\n";
			print'<BR><INPUT TYPE="submit" VALUE="Submit Changes"></FORM><br>' . " \n ";
			mysql_shutdown_next();
			print '</td></tr></table>';
		}
		break;

		case "updat": 		// Updating
		if ($debug )
		{
			print '<p> functions02.inc::edit_element(update)';
			print '<br>$id = ' .$id ;
			print '<br>$_GET[\'action\'] = ' .$_GET['action'];
			print '<br>$portal_user = '.$portal_user ;
			print '<br>$table= '.$table.'<br><br>' . "\n";
		}
		if ( $_POST['frm_reason'] == '')
		{
			print "<FONT CLASS=\"warn\"> No reason given for the update of $element: '$id'. <BR>Try again.</FONT>"; print "\n";
			edit_element($id, "edit",$table ,$page);  print "\n";
		}
		elseif ( $_POST['frm_description'] == '') //We need the Problem Description field not empty
		{
			print "<FONT CLASS=\"warn\"> Missing Problem Description for the update of $element: '$id'. <BR>Try again.</FONT> \n";
			edit_element($id, "edit",$table,$page);  print "\n";
			print '<p>frm_scope="'. $_POST['frm_description'] .'"';
			print '<p>frm_affected="'. $_POST['frm_affected'] .'"';
			print '<p>frm_owner="'. $_POST['frm_owner'] .'"';
			print '<p>frm_location="'. $_POST['frm_location'] .'"';
		}
		elseif ($id == 0)
		{
			//Insert $Element
			$query = make_insert_query($table, "insert",$id); // Call make_insert_query() to build the quey.
			$result = mysql_query($query) or do_error('functions02.inc::edit_element:update', 'mysql_query() failed', mysql_error);
			$new_id = mysql_insert_id() ;
			if ($debug)
			{
				print '<br>functions02.inc::edit_element(update):: You just inserted ID '. $new_id ;
			}
			//mysql_shutdown_next();
			// Time to do Add New Element.
			print '<table><tr><td width="30%">';
			print '<p  CLASS="severity_medium">New '. $element. ' has been added.  ';
			print '</td></tr></table>'; print "\n";
			// Send email with text of $element
			if ($GLOBALS['SEND_EMAIL'])
			{
				$email_text = build_email_element($new_id ,$table);
				if ($debug)
				{
					$frm_problemstart = $_POST['frm_year_problemstart'] .'-'. $_POST['frm_month_problemstart'] .'-'. $_POST['frm_day_problemstart'] .' ';
					$frm_problemstart = $frm_problemstart  . $_POST['frm_hour_problemstart'] .':'. $_POST['frm_minute_problemstart'] .':00';
				}
				if (trim(get_variable("ticket_website")) != "")
				{
					$email_text = $email_text . " \n " . 'Permanent link: '.get_variable("ticket_website").'show.php?id='.  $new_id;
				}
				send_email($email_text);
			}

			//print '<br><br><br>';
			//print '<p align="center"><FORM METHOD="post" ACTION="'.$page.'.php?action=add&id=0" target="main">';
			//print '<INPUT TYPE="submit" VALUE="Add a new '.$element.'"></FORM></p>';print "\n";
			// Insert the reason with Transaction Management: log user, date, time, changes.
			if ($debug)
			{
				 /***
				 print '<p class="grey">From user:<b>'.$portal_user.'</b> , on '. date("H:i, jS F, Y").
				'<br> <font class="severity_medium">Inserting the reason:</font> '.$_POST['frm_reason'] .'<BR><BR>';   print "\n";
				***/
				/*****
				transaction_management($id, $table, $action,$_POST['frm_reason'], $portal_user);   print "\n";
				*****/
			}
			//show_data($id);
			list_data($table, "",0,0,$element);
			mysql_shutdown_next();
			return 0; // All should be allright now.
		}else
		{
			// update $element
			if ($id == '' OR $id < 0)// OR !check_for_rows("SELECT * FROM $table WHERE id='$id'") )
			{
				if ($debug )  print '<p>functions02.inc::edit_element(update)<br> Invalid id. $id = ' .$id .
						'\t\$_GET[\'action\'] = '.$_GET['action'].'	|| $portal_user = '.$portal_user .'<br><br>';
				print "<FONT CLASS=\"warn\">Invalid $element: '$id' </FONT>";   print "\n";
				if (trim($element)=="") { $element = trim($table) ;}
				show_data($id,$table,$element."s",$page); // ($id,$table,$title,$page)   print "\n";
			} else
			{
				// Time to do updates.
				print '<FONT CLASS="severity_medium">'.$element.' has been updated</FONT><BR><BR>';  print "\n";
				 /***
				// Insert the reason with Transaction Management: log user, date, time, changes.
				print '<p class="grey">From user:<b>'.$portal_user.'</b> , on '. date("H:i, jS F, Y").
				'<br> <font class="severity_medium">Inserting the reason:</font> '.$_POST['frm_reason'] .'<BR><BR>';   print "\n";
				//transaction_management($id, $table, $action,$_POST['frm_reason'], $portal_user);  print "\n";
				print '<p class="severity_medium"> Inserting the reason:<br><font class="grey">From user:<b>'.$portal_user.'</b> ,
						on '. date("H:i, jS F, Y").'<br>'.$_POST['frm_reason'] .'</font><BR><BR>';   print "\n";
				transaction_management($id, $table, $action,$_POST['frm_reason'],$portal_user);		  print "\n";
				******************/
				//Update $Element function make_insert_query($table, $type,$id,$visit=0,$aid=0)
				$query=make_insert_query($table, "update", $id);
				// On error, exit.
				$result = mysql_query($query) or do_error('functions02.inc::edit_element(updat)::mysql_query()', 'mysql query failed', mysql_error());
				// show updated $element
				show_data($id, $table,"$element" . 's',$page);
			}
		}
		break;

		// Deleting
		case "delet":
		if ($debug )  print '<p>functions02.inc::edit_element(delete)<br>$_POST[frm_confirm] = ' . $_POST['frm_confirm'] ;
		if ($_POST['frm_confirm'])
		{
			if ( $_POST['frm_reason'] == '')
			{
				print "<P CLASS=\"warn\"> No reason given for removal of $element: '$id'. <BR>Try again.</P>";  print "\n";
				show_data($id,$table, $element,$page);	  print "\n";
			}else
			{
				if ($debug )
				{
					print '<p>functions02.inc::edit_element(delete).';
					print '<br>$id = ' .$id ;
					print  '<br> \$_GET[\'action\'] = ' .$_GET['action'] ;
					print  '<br>$portal_user = '.$portal_user ;
					print  '<br>\$_POST[\'frm_confirm\'] = '.$_POST['frm_confirm'].'<br><br>'."\n";
				}
				// remove $element and make sure its logged into the database
				//transaction_management($id, $table, $action,$_POST['frm_reason'], $portal_user);  print "\n";
				$result = mysql_query("DELETE FROM $table WHERE ID='$id'")
					or do_error('functions02.inc::edit_element(delete)', 'mysql_query() failed', mysql_error());
				print "<P CLASS=\"header\">$element number '$id' has been removed.</P><BR><BR>";	  print "\n";
				list_data($table, "",0,0,$element);	  print "\n";
			}
		}
		else
		//confirm deletion
		{
			$query = "SELECT * FROM $table WHERE id='$id'";
			if ($debug )
			{
				print '<p>functions02.inc::edit_element(delet)';
				print '<br>$id = ' .$id ;
				print '<br> \$_GET[\'action\'] = '.$_GET['action'];
				print '<br> $query = '.$query;
				print '<br> $portal_user = '.$portal_user .'<br><br>'."\n";
			}
			$result = mysql_query($query) or
				do_error('functions02.inc::edit_element:delet', 'mysql_query() failed', mysql_error());
			$row = mysql_fetch_array($result);

			if ($debug )
			{
				print '<p>functions02.inc::edit_element(delete)';
				print '<br>$id = ' .$id;
				print '<br> \$_GET[\'action\'] = '.$_GET['action'];
				print '<br>$portal_user = '.$portal_user;
				print '<br>\$_POST[\'frm_confirm\'] = '.$_POST['frm_confirm'].'<br><br>'."\n";
			}

			print '<FONT CLASS="severity_medium">Confirm deletion of '.$element.' #.'. $id;
			print '<br>"'.$row[1].', '.$row[2].', '.$row[3].', '.$row[4].'" </FONT>';
			print '<BR><BR><FORM METHOD="post" ACTION="'.$page.'.php?id='.$id.'&action=delet&go=1">'. " \n \n ";
			print '<TABLE BORDER="0">';
			print '<TR><TD CLASS="td_greylabel">Delete '.$element.' #'.$id.' ?&nbsp;</TD>
				<TD><INPUT TYPE="checkbox" NAME="frm_confirm" VALUE="1">&nbsp;&nbsp;Yes, delete this '.$element.'</TD>
				</TR>' . " \n ";
			print '<TR>
				<TD CLASS="td_greylabel">Provide a Reason for<br>deleting the '.$element.':</TD>
				<TD><TEXTAREA NAME="frm_reason" COLS="40" ROWS="4">'.$reason.'</TEXTAREA></TD></TR>'. " \n  \n ";
			print '</TABLE><br>'. " \n ";

			print '<table width="300"><tr>';
			print '<td width="50%" align="left">';
			print '<INPUT TYPE="Submit" VALUE="Confirm">';
			print '</FORM></td>' . "\n";
			print '<td width="50%" align="right">';
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php"><INPUT TYPE="submit" VALUE="Cancel"></FORM></td> ' . "\n";
			print '</tr></table>';

		}
		break;

		// Showing info, so we can edit the data.
		case " ":
			print  "<P CLASS=\"warn\"> No action given. Stop.</p>". "\n";
		break;
	}
}

/***********************************************************************
*** function list_data($table,$SQL=" WHERE 1 ",$current=0,$search=0) ***
*** Displays the selected table, sortable ASC and DESC by  *************
*** any of the table's fields. *****************************************
*** $search means this is a the result of user's search ****************
***********************************************************************/
function list_data($table,$SQL=" WHERE 1 ",$current=0,$search=0,$printing=0)
{
	// $table : table to use
	// $SQL : extra SQL code, like searches. Defaults as " WHERE 1 " when no extra SQL needed
	// $current is current position in shown results  (defaults to first element)
	// $search : is this a search? 0 = no / 1 = yes
	// $printing: is this intended to print data? 0 = no / 1 = yes 
	global $debug;
	global $per_page;
	if ($per_page == 0)  $per_page=20;  // Limit results shown to 20 results per page
	$ident = 0;
	// On error, exit.
	if (! $table ) 	{ print "<P class=\"severity_medium\"> Error! no variable given in  list_data(\$table)!"; return 0; }
	//Ternary operator, sort order, ascending/descending
	if ($debug) // For Debug
	{
		print "<p>functions02.inc::list_data() pre \$table is ".$table ." <br>";
		print "functions02.inc::list_data() pre \$_GET['order'] is ".$_GET['order'] ." <br>";
		print "functions02.inc::list_data() pre \$_GET['sortby'] is ".$_GET['sortby'] ." <br>";
		print "functions02.inc::list_data() pre \$_GET['ident'] is ".$_GET['ident'] ." <br>";
	}

	// Search for one single element number
	if ($_GET['action'] =="")
	{
		search_by_element_num($GLOBALS['ELEMENT']);
	}
	$SQL=" WHERE 1 ";
	$order = ($_GET['order'] == ('')) || ($_GET['order'] == ('DESC'))  ? 'DESC' : 'ASC';
	// Ternary operator that decides sort order on click.
	$sortby = $_GET['sortby'] == '' ? 'id ' : $_GET['sortby'] ;
	// Get current starting threshold. If there's none, equals zero.
	$current = $_GET['current'] == '' ? 0 : $_GET['current'];
	// Get $element to display, by severity: all (0), medium severity or higher (1+) or high severity only (2)
	$disp_severity = $_GET['severity'] == '' ? 0 : $_GET['severity'];
	$SQL = $SQL . " AND `severity` >=" .$disp_severity ." ";
	// Show only closed tickets? (status_closed)
	//$status = ($_GET['showclosed'] == 'true') ? $GLOBALS['STATUS_CLOSED'] : $status = $GLOBALS['STATUS_OPEN'];
	if ($_GET['showclosed'] == 'true') // If closed
	{
		//$status =$GLOBALS['STATUS_CLOSED'];
		$status =" `status` = ".$GLOBALS['STATUS_CLOSED'] ;
		$isclosed = "&showclosed=true";
	} else // If not closed (open, waiting)
	{
		//$status=$GLOBALS['STATUS_OPEN'];
		$status =" `status` != ".$GLOBALS['STATUS_CLOSED'] ;
		$isclosed = "";
	}
	if ($_GET['unassigned'] == $GLOBALS['STATUS_UNASSIGNED']) // If show unassigned only
	{
		$unassigned = " AND `owner` = '1' ";
		$SQL = $SQL . " AND  $status " . $unassigned;
	} else if ($_GET['unassigned'] == $GLOBALS['STATUS_ASSIGNED']) // If show assigned only
	{
		$unassigned = " AND `owner` != '1' ";
		$SQL = $SQL . " AND  $status " . $unassigned;
	} else // If show all
	{
		$SQL = $SQL . " AND  $status ";
	}
	//$SQL = $SQL . " AND `status` = $status ";

	// SQL query --> How many rows do we have in total?
	$query_max = "SELECT `id` FROM `$table` ".$SQL . " " ;  // Select ids from table (fast query)
	if ($debug) // For Debug
	{
		print "<p>functions02.inc::list_data() post \$order is $order <br>";
		print "functions02.inc::list_data() post \$sortby is $sortby  <br>";
		print "functions02.inc::list_data() post \$ident is $ident  <br>";
		print "functions02.inc::list_data() \$_GET[current] is ".$_GET['current']." <br>"; // For Debug
		print "functions02.inc::list_data() \$_GET[severity] is ".$_GET['severity']." <br>"; // For Debug
		print "functions02.inc::list_data() \$_GET[showclosed] is ".$_GET['showclosed']." <br>"; // For Debug
		print "functions02.inc::list_data() post \$status is $status  <br>";
		print "functions02.inc::list_data() \$query_max(maxnum) = $query_max <br>";
		print "functions02.inc::list_data() \$isclosed = '$isclosed' <br>";		
	}
	$result_max = mysql_query($query_max) or
			do_error('functions02.inc::list_data()::mysql_query()', 'mysql query failed with query_max ' . $query_max, mysql_error());
	$maxrows=mysql_num_rows($result_max);

	// Build real SQL query
	$query = "SELECT * FROM `$table` ". $SQL. " ORDER BY $sortby $order LIMIT ".$current.",$per_page";

	$result = mysql_query($query) or
			do_error('functions02.inc::list_data()::mysql_query()', 'mysql query failed with query ' . $query, mysql_error());

	// Dual color initialization for information display in "log paper" style.
	$tblcolor = "td_grey";
	print "<!-- Continuing table of Hosts table. Now, the data, bi-colour -->";
	// Fetch results and get highest id
	$high=1;

	// As per hack developed with help from iarenaza@escomposlinux.org
	// for function transaction_management($id, $table, $type, $reason, $portal_user)
	/*********************************************************************************
	$row = mysql_fetch_array($result, MYSQL_ASSOC);
	**** The previous line gets commented out b/c it was making php read a row, and **
	**** this row was never used,efectively not showing up on the resulting grid. ****
	*********************************************************************************/
	if ($debug)
	{
		print "functions02.inc::list_data() \$query = $query <br>";
		print "<p>functions02.inc::list_data() || I'm out: Starting generation of elements' table<br>";
	}
	// If we are printing, we do not need index whatsoever nor links
	if ($printing == 0)
	{
		$index_of_pages = index_elements($maxrows,$current,$per_page,$order,$SQL,$isclosed);
	}
	// (previous line) Index of elements shown. Show $per_page elements at a time
	print "<table  width=\"750\" border=\"0\">"; // Start Results table.

// Search if there is a limited number of fields to show OR if there is special order of fields to be shown.
	global $table_fields;
	which_fields($table); // filling $table_fields

if (sizeof($table_fields) == 0) // Should we display all fields? or only a subset? If sizeof()== 0, All fields
{
	if ($debug)
	{
		print '<h3>There is NO stuff in which_fields()</h3>';
		print '<p>functions02.inc::list_data(). "table" is: ' . $table;
	}
	if ($row = mysql_fetch_array($result)) // As we use this fetch_array, we will use do{} instead of while{}
	{
		// Switch between ASC and DESC sort order. Thanks to coder <nerv@ono.com>, from escomposlinux.org for the idea
		// that made possible the next line of code, for ASC/DESC alternance.
		//$order = isset( $_GET['order'] ) == 1    ? '' : 'DESC';
		// Commenting out previous code b/c it's wierd and doesn't work as expected.
		if ( ($_GET['order'] == ('')) OR ($_GET['order'] == ('DESC')) ) $order = "ASC";
		else $order = "DESC";
		// We already have $order defined, checked against $_GET['order'] , empty or not.
		// $order = $order == "ASC" ? "DESC" : "ASC";
		// Arghhhhhh. All previous versions, have foobar'ed the working version.
		// This next one is the working version (please!)
		//$order = isset( $_GET['order'] ) == 1    ? '' : '&order=DESC';

		//Print clickable headers first
		// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
		$iflds = mysql_num_fields($result) - 1;		// Number of fields
		$irows = mysql_num_rows($result); // number of rows
		// print '<p> Num of rows is '.$irows.' ';
		// Initialize array with obvious wrong values, to detect errors.
		$fieldnames = array("long", "day","at","the", "races");
		//Request fildnames, in long version
		if($debug)
		{
			print "<p>functions02.inc::list_data() || I'm in: inside table generation";
			print'<br>functions02.inc::list_data(): $order is '. $order ;
			print'<br>functions02.inc::list_data(): number of fields is '. $iflds ;
			print'<br>functions02.inc::list_data(): number of rows is '. $irows ;
			print'<br>functions02.inc::list_data()::fieldnames call: $table is '. $table ;
		}
		$fieldnames = get_fieldname(0, $table); // 0 => short. !0 => long
		print "  <TR>";  // print row for titles
		for ($i = 0; $i <= $iflds; $i++)
		{
			// This is the field name.
			$ident = (mysql_field_name($result, $i));
			// Print headers with a link to field name, for sorting pourposes.
			// "function result_header_hosts()" is *not* nedded with this new code.
			// This next line was foobaring the ASC/DESC sorting for some reason.
			//print '<TH class="thead"><A HREF="?sortby='. $ident . '&order='.$order .'&search='.$search.'" CLASS="td_link">'
			//		.$ident.'</A></TH>';
			// This is an old version, and it worked. And it works now. Added $search part and it works again.
			// If we are printing, we do not need index whatsoever nor links
			if ($printing == 0)
			{
				switch ($i)
				{
				case "0": // id number, plus last update
					print '<TH class="thead"><A HREF="?sortby='. $ident. $isclosed .'&order='. $order;
					print '&search='.$search.' " CLASS="td_link">&nbsp;&nbsp;'.$fieldnames[$i].'&nbsp;&nbsp;</A></TH>';
					print '<TH class="thead">&nbsp;&nbsp;Last Update&nbsp;&nbsp;</TH>';
				break;
				case "2": // Short Fields
				case "5":
					print '<TH class="thead"><A HREF="?sortby='. $ident. $isclosed .'&order='. $order;
					print '&search='.$search.' " CLASS="td_link">&nbsp;&nbsp;'.$fieldnames[$i].'&nbsp;&nbsp;</A></TH>';
				break;
				default:
					print '<TH class="thead"><A HREF="?sortby='. $ident. $isclosed .'&order='
					. $order .'&search='.$search.' " CLASS="td_link">'.$fieldnames[$i].'</A></TH>';
				break;
				}
			} else {
				switch ($i)
				{
				case "0": // id number, plus last update
					print '<TH class="thead">&nbsp;&nbsp;'.$fieldnames[$i].'&nbsp;&nbsp;</TH>';
					print '<TH class="thead">&nbsp;&nbsp;Last Update&nbsp;&nbsp;</TH>';
				break;
				case "2": // Short Fields
				case "5":
					print '<TH class="thead">&nbsp;&nbsp;'.$fieldnames[$i].'&nbsp;&nbsp;</TH>';
				break;
				default:
					print '<TH class="thead">'.$fieldnames[$i].'</TH>';
				break;
				}
			}
		}
		print " </TR>"; // End of titles' row
		// We already have fetched an array of data. Let's do "do{...}while()" instead of "while(){...}"
		do { //Now, it's "display-the-data" time,  One element per line. :)
				// Make $high held the highest value of $row[0] a.k.a. $row["id"]
				$high > $row[0] ? $high = $high : $high = $row[0];
				// First, get a link in the 'id' column.
				print '<TR >';
				// If we are printing, we do not need index whatsoever nor links
				if ($printing == 0)
				{
					print "<TD class=\"$tblcolor\"><A HREF=\"?id=$row[0]\">#$row[0]</a></TD>";
					print "<TD class=\"$tblcolor\"><A HREF=\"?id=$row[0]\"> get_last_updated($row[0])</a></TD>";
				} else {
					print '<TD class="'.$tblcolor.'">#'.$row[0].'</TD>';
					print '<TD class="'.$tblcolor.'">'. get_last_updated($row[0]).'</TD>';
				}
				//Then, print the rest.
				for ($i =1; $i <= $iflds; $i++)
				{
					switch ($i)
					{
					case "2": // Short Fields
					case "3":
						print '<TD class="'.$tblcolor.'" align="center">'.$row[$i].'</TD>';
					break;
					default:
						print '<TD class="'.$tblcolor.'">'.$row[$i].'</TD>';
					break;
					}
				}
				print "  </TR>\n";
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
			print "<p>functions02.inc::list_data() || I'm in the else<br>";
			print'<p>functions02.inc::list_data(): number of fields is '. $iflds ;
			print'<br>functions02.inc::list_data(): number of rows is '. $irows ;
		}
		print '<p class="severity_medium"> No results were found (Num of rows is '.$irows.' )';
	}
} else { // If there is only a subset of fields to show.
	if ($debug)
	{
		print '<h3>There is stuff in which_fields() : ' . sizeof($table_fields) .'</h3>';
		$i = 0;
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
			print '<p>functions02.inc::list_data() || I\'m in: inside table generation<br>';
			print'<br>functions02.inc::list_data(): $order is '. $order ;
			print'<br>functions02.inc::list_data(): number of fields is '. $iflds ;
			print'<br>functions02.inc::list_data(): number of rows is '. $irows ;
			print'<br>functions02.inc::list_data()::fieldnames call: $table is '. $table ;
		}
		$fieldnames = get_fieldname(0, $table); // 0 => short. !0 => long
		print "  <TR>";  // print row for titles

		while( $element = each($table_fields)) // [1] contains the proper 'value'. [0] contains 'order'
		{
			// This is the field name.
			$ident = (mysql_field_name($result, $element[1]));
			// Print headers with a link to field name, for sorting pourposes.
			// If we are printing, we do not need index whatsoever nor links
			if ($printing == 0)
			{
				switch ($element[1])
				{
				case "0": // id number, plus last update
					print '<TH class="thead"><A HREF="?sortby='. $ident. $isclosed .'&order='. $order;
					print '&search='.$search.' " CLASS="td_link">&nbsp;&nbsp;'.$fieldnames[$element[1]].'&nbsp;&nbsp;</A></TH>';
					print '<TH class="thead">&nbsp;&nbsp;Last Update&nbsp;&nbsp;</TH>';
				break;
				case "7": // Short Fields:  Status, Owner, Severity, Location
				case "8":
				case "9":
				case "10":
					print '<TH class="thead"><A HREF="?sortby='. $ident. $isclosed .'&order='
					. $order .'&search='.$search.' " CLASS="td_link">&nbsp;&nbsp;'.$fieldnames[$element[1]].'&nbsp;&nbsp;</A></TH>';
				break;
				default:
					print '<TH class="thead"><A HREF="?sortby='. $ident. $isclosed .'&order='
					. $order .'&search='.$search.' " CLASS="td_link">'.$fieldnames[$element[1]].'</A></TH>';
				break;
				}
			} else {
				switch ($element[1])
				{
				case "0": // id number, plus last update
					print '<TH class="thead">&nbsp;&nbsp;'.$fieldnames[$element[1]].'&nbsp;&nbsp;</TH>';
					print '<TH class="thead">&nbsp;&nbsp;Last Update&nbsp;&nbsp;</TH>';
				break;
				case "7": // Short Fields:  Status, Owner, Severity, Location
				case "8":
				case "9":
				case "10":
					print '<TH class="thead">&nbsp;&nbsp;'.$fieldnames[$element[1]].'&nbsp;&nbsp;</TH>';
				break;
				default:
					print '<TH class="thead">'.$fieldnames[$element[1]].'</TH>';
				break;
				}
			}
		}
		print " </TR>"; // End of titles' row

		// We already have fetched an array of data. Let's do "do{...}while()" instead of "while(){...}"
		do { //Now, it's "display-the-data" time,  One element per line. :)
			// Make $high held the highest value of $row[0] a.k.a. $row["id"]
			$high > $row[0] ? $high = $high : $high = $row[0];
			// First, get a link in the 'id' column. Then, print the rest.
			/*  */
			reset($table_fields);
			print '<TR >';
			while( $element_data = each($table_fields)) // [1] contains the proper 'value'. [0] contains 'order'
			{
				//color tickets by severity
				switch($row[9])
				{
					case $GLOBALS['SEVERITY_MEDIUM']: $severityclass='severity_medium'; break;
					case $GLOBALS['SEVERITY_HIGH']: $severityclass='severity_high'; break;
					default: $severityclass=''; break;
				}

				if ($debug)
				{
					print "<br>current element_data is $element_data[1]";
				}

				switch ($element_data[1])
				{
				case "0": // Short Field: Num,  **PLUS**  Last Update. Limited to "yyyy-mm-dd hh:mm"
					// If we are printing, we do not need index whatsoever nor links
					if ($printing == 0)
					{
						print '<TD class="'.$severityclass.'"  nowrap ><A HREF="?id='.$row[0].'">#'.$row[0].'</a></TD>';
						print '<TD class="'.$severityclass.'"><A HREF="?id='.$row[0].'">'. substr(get_last_updated($row[0]),0,16).'</a></TD>';
					} else {
						print '<TD class="'.$severityclass.'" nowrap >#'.$row[0].'</TD>';
						print '<TD class="'.$severityclass.'">'. substr(get_last_updated($row[0]),0,16).'</TD>';
					}
				break;
				case "5":
				//do abbreviations on affected if neccesary
					if (get_variable('abbreviate_affected'))
					{
						$wrap_var=" NOWRAP ";
						if (strlen($row[5]) > get_variable('abbreviate_affected'))
						{
							$row[5] = substr($row[5],0,get_variable('abbreviate_affected')).'...';
						}
					}
					print '<TD class="'.$severityclass.'" '.$wrap_var.'>'.$row[5].'</TD>';
					$wrap_var="";
				break;
				case "6":   // Description
				//do abbreviations on description if neccesary
					if (get_variable('abbreviate_description'))
					{
						$wrap_var=" NOWRAP ";
						if (strlen($row[6]) > get_variable('abbreviate_description'))
						{
							$row[6] = substr($row[6],0,get_variable('abbreviate_description')).'...';
						}
					}
					$i = $element_data[1];
					print '<TD class="'.$severityclass.'" align="left"  '.$wrap_var.'><A HREF="?id='.$row[0].$isclosed.'">'.$row[6].'</a></TD>';
					$wrap_var="";
				break;
				case "7":  // Short Fields: Status
					print '<TD class="'.$severityclass.'" align="center" nowrap>'.get_single_option("status",$row[$element_data[1]]).'</TD>';
				break;
				case "10":  // Short Fields:  Location
					print '<TD class="'.$severityclass.'" align="center" nowrap>'.get_single_option("location",$row[$element_data[1]]).'</TD>';
				break;
				case "8":// Short Fields: Owner
					print '<TD class="'.$severityclass.'" align="center" nowrap>'. get_owner($row[8]).'</TD>';
				break;
				case "9":   // Short Fields: Severity
					print '<TD class="'.$severityclass.'" align="center" nowrap>'.get_single_option("severity",$row[$element_data[1]]).'</TD>';
				break;
				default: // Only show first 10 chars, like day
					print '<TD class="'.$severityclass.'">'.substr($row[$element_data[1]],0,10).'</TD>';
				break;
				}
			}
			print "  </TR>\n";/* */
		 } while($row = mysql_fetch_array($result));
	}  else
	{
		// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
		$iflds = mysql_num_fields($result) - 1;		// Number of fields
		$irows = mysql_num_rows($result); // number of rows
		if($debug)
		{
			print '<p>functions02.inc::list_data() || I\'m in the else<br>';
			print'<br>functions02.inc::list_data(): number of fields is '. $iflds ;
			print'<br>functions02.inc::list_data(): number of rows is '. $irows ;
		}
		print '<p class="severity_medium"> No results were found (Num of rows is '.$irows.' )';
	}
}

	print "</table>\n <br>"; // Finish table
	// If we are printing, we do not need index whatsoever nor links
	if ($printing == 0)
	{	
		print $index_of_pages; // This substitutes previous line. Hopefuly, now index with proper ASC/DESC will work.
	}
	if($debug)
	{
		print'<br>functions02.inc::list_data(): $high is '. $high;
	}
	return $high; //Yeah, we will be high after all this code.
}




/*************************************************************
***  function own_elemtn($id) ********************************
*** Change ownership of a $element ***************************
*************************************************************/
function own_element($id)
{
	global $debug;
	if ($_GET['action'] != "own")
	{
		print '<table  bgcolor="white"><tr><td valign="top" width="20%">';	  print "\n";
		print '</td></tr><tr><td valign="top" width="75%">';  print "\n";
		print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].".php?id=$id&action=ownupdat\">";print "\n";
		print '<p><INPUT TYPE="hidden" NAME="frm_reason" VALUE="not empty"></TD>';
		print '<p><INPUT TYPE="hidden" NAME="frm_owner" VALUE="'.$_SESSION['ticket_username'].'"></TD>';
		print'<BR><INPUT TYPE="submit" VALUE="Submit Changes"></FORM><br>';		print "\n";
		print '</td></tr></table>';
	} if ($_GET['action'] != "ownupdat")
	{
		$SQL = "UPDATE `".$GLOBALS['MAIN_TABLE']."` SET `owner` = '".get_id_from_user($_SESSION['ticket_username']);
		$SQL = $SQL ."' WHERE `id` =$id LIMIT 1";
		print '<FONT CLASS="severity_medium">You have taken ownership of '.$GLOBALS['ELEMENT'].' '.$id .'</FONT><BR><BR>';
		print "\n";
		//Update $Element
		if ($debug)
		{
			print '<p>functions02.inc::own_element(ownupdat)';
			print '<br>$SQL="' . $SQL . '"';
			print '<br>$_SESSION[username]="' . $_SESSION['ticket_username'] . '"';
			print '<br>$id="' . $id . '"';
			print '<br>$GLOBALS[ELEMENT]="' . $GLOBALS['ELEMENT'] . '"' . " \n ";

		}
		$result = mysql_query($SQL) or
			do_error('functions02.inc::own_element(ownupdat)::mysql_query()', 'mysql query failed', mysql_error());
		// show updated $element
		//show_data($id, $GLOBALS['MAIN_TABLE'],$GLOBALS['ELEMENT'] . 's',$GLOBALS['MAIN_PAGE']);
		list_data($GLOBALS['MAIN_TABLE'], "",0,0,$GLOBALS['ELEMENT']);
	}
	mysql_shutdown_next();


}


/*************************************************************
***  function  search_by_element_num($element) ***************
*** Display little "Search by $element Number" box ***********
*************************************************************/
function search_by_element_num($element)
{
	print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php">' ."\n";
	print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
	print '<TR><TD VALIGN="top" CLASS="td_label">Search by '.$element.' Number (Num.) &nbsp;</TD>' ."\n";
	print '<TD><INPUT TYPE="text" SIZE="4" MAXLENGTH="4" VALUE="" NAME="id"></TD>' ."\n";
	print '<TD><INPUT TYPE="submit" VALUE="Go"></TD></TR>' ."\n";
	print '</TABLE></FORM>' ."\n";
}


/*************************************************************
***  function show_data($id,$table,$name,$page,$start_id) ****
*** Show the selected $element, building all dinamicaly ******
*************************************************************/
function show_data($id,$table,$name,$page,$start_id=0)
{
	global $debug;
	// Remove trailing 's' from $title.
	// Begin the webpage
	do_title("Showing $name #$id");
	$name = $GLOBALS['ELEMENT']	;
	//For Debugging
	if ($debug)
	{
		print " \n <p>functions02.inc::show_data(): Showing $name $id \n";
		print "<br> id = $id";
		print "<br> table = $table";
		print "<br> name = $name";
		print "<br> page = $page";
		print "<br> start_id = $start_id \n ";

	}

	/* sanity check */
	if ($id == '' OR $id <= 0 OR !check_for_rows("SELECT * FROM $table WHERE id='$id'"))
	{
		print "<p>functions02.inc::show_data()<br>Invalid $name id: '$id'<br>\n";
		return;
	}
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $table); // 0 => short. !0 => long
	// Build the SQL query
// This will be the fieldname --> $fieldnames[$i]
	// Retrieve the field-names of $table from the database $mysql_db_next
	global $link_next ;
	global $mysql_db_next;
	$fields = mysql_list_fields($mysql_db_next, $table, $link_next);
	$columns = mysql_num_fields($fields);

	// Build the SQL query
	$query = "SELECT * FROM `$table` WHERE `id`='$id'";
	if ($debug)
	{
		print " \n <br>show_data(): query is '$query' \n";
	}
	$result = mysql_query($query) or
		do_error('functions02.inc::show_data():mysql_query()', 'mysql query failed', mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0" bgcolor="white"> <tr><td  width="25%" valign="top">'; 	  print "\n";
	add_element_buttons($id, $name );
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
			print " \n <br>show_data(): \$i is '$i' \n";
			print " \n <br>show_data(): \$fieldnames[\$i] is '$fieldnames[$i]' \n";
			print " \n <br>show_data(): \$row[\$i] is '$row[$i]' \n";
		}
		print '</TD>';  print "\n";
		print ' <TD width="75%">';
		switch ($i)
		{
			case (6): // Description
				print '<pre
					style=" white-space: pre-wrap; /* css-3 */ white-space: -moz-pre-wrap; /* Mozilla, since 1999 */
					white-space: -pre-wrap; /* Opera 4-6 */ white-space: -o-pre-wrap;    /* Opera 7 */ word-wrap: break-word; ">';
				print $row[$i];
				print '</pre>';
			break;
			case (7): //Status
				if ($debug)
				{
					print " \n <br>get_single_option is '".get_single_option("status", $row[$i])."'\n";
				}
				print get_single_option("status", $row[$i]);
			break;
			case (8): // Owner
				print get_owner($row[$i]);
			break;
			case (9): // Severity
				print get_single_option("severity", $row[$i]);
			break;
			case (10): // Location
				print get_single_option("location", $row[$i]);
			break;
			default: // The Rest
				print $row[$i];
			break;
		}
		print ' </TD></TR>';	  print "\n";
	}
	print '</TABLE>';	 print "\n";

	if ($row[8] == 1)  // If ticket is un-assigned
	{
		print '<table>';
		print '<tr><td>';
		print '<p><FORM METHOD="post" ACTION="'.$page.'.php?action=own&id='.$id.'">';print "\n";
		print'<p class="severity_medium"><INPUT TYPE="submit" VALUE="Take Ownership of this '.$name.'"> &nbsp; Click Here to take ownership of this '.$name .' </FORM> </p>';		print "\n";
		print '</td></tr>';
		print '</table>';
	}
	print '</td></tr></TABLE>';	  print "\n";

	mysql_shutdown_next();
	return 0;
}




/***********************************************************************
*** function do_something() ********************************************
*** Work needs to be done  *********************************************
***********************************************************************/
function do_something()
{
	print '<p>Nothing here -yet-';
	powered("1");
}

?>
