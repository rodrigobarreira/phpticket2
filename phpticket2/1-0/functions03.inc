<?php
/**************************************
*** File: functions03.inc  ********
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

/*************************************************************
*** function add_action_buttons($id, $page, $name) *********
*** add Buttons to Patients info *****************************
*************************************************************/
function add_action_buttons($option_id, $element_id="")
{
	global $debug;
	if ($debug)
	{
		print '<p>functions03.inc::add_action_buttons()';
		print '<br>$option_id="'.$option_id.'"';
		print '<br>$_SESSION[user_is_admin]="'.$_SESSION['ticket_user_is_admin'].'"';
	}
	if ($_SESSION['ticket_user_is_admin'] > 0)
	{
		$url = $GLOBALS['SECOND_PAGE'].'.php';
		print '<table cellpadding="10" width="75%"><tr>';
		//print '<td><p  class="severity_medium">Options Menu </p></td>';
		print '<td><FORM METHOD="post" ACTION="'.$url.'?id='.$option_id.'&action=edit">';print "\n";
		print '<INPUT TYPE="submit" VALUE="Edit '.$GLOBALS['OPTION'] .'\'s Data"></FORM> </td>';		print "\n";
		print '<td><FORM METHOD="post" ACTION="'.$url.'?id='.$option_id.'&action=delet">'; print "\n";
		print '<INPUT TYPE="submit" VALUE="Delete '.$GLOBALS['OPTION'] .'"></FORM> </td>';		print "\n";

		if ($element_id != "")
		{
			print '<td><FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php?id='.$element_id.'">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Go Back to Ticket '.$element_id.'"></FORM> </td>';		print "\n";
		} else {
			/****************
			print '<td><FORM METHOD="post" ACTION="'.$url.'">'; print "\n";
			print '<INPUT TYPE="submit" VALUE="Go Back to Main Page"></FORM> </td> ';		print "\n";
			********************/
			print '<td>';
			go_back($url);
			print '</td>';
		}
		print "</tr></table> \n";
	}
}


/*************************************************************
*** function build_add_options($id,$table,$element_id) **********
*** Builds a form on-the-fly for adding the retrieved data **
*************************************************************/
function build_add_options($id,$element_id="")
{
	global $debug;
	// Retrieve the field-names of $table from the database $mysql_db_next
	global $link_next ;
	global $mysql_db_next;
	$fields = mysql_list_fields($mysql_db_next, $GLOBALS['OPTION_TABLE'], $link_next);
	$columns = mysql_num_fields($fields);
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $GLOBALS['OPTION_TABLE']); // 0 => short. !0 => long
	// Build the SQL query
	$query = "SELECT * FROM ".$GLOBALS['OPTION_TABLE']." WHERE 1 LIMIT 1";   // ID='$id'";
	//For Debugging
	if ($debug)
	{
		print '<p> functions03.inc::build_add_options()';
		print '<br>$id= '.$id.'<br>$table= '.$GLOBALS['OPTION_TABLE'] .'<br>$element_id'.$element_id;
		print '<br> $query= '. $query .'<br><br>' ;
	}
	$result = mysql_query($query) or
			do_error('functions03.inc::build_add_options()::mysql_query()', 'mysql query failed', mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0" bgcolor="white">';		print "\n";
	// Fields start in $i=0
	//$i=0 is "action_id", as 'aid' is an **auto** field, we skip it.
	//$i=1 is "ticket_id", as 'tid' is an **auto** field, we skip it too.
	$tab_value = 1;
	for ($i = 1; $i < $columns; $i++)
	{
		switch ($i)
		{
		case (1): // option_id
			print '<TR>';
			print '<TD align="center" COLSPAN="3" CLASS="td_label" valign="top" NOWRAP>'. $GLOBALS['OPTION'].' Text:</TD>' . "\n";
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';//NAME="frm_ticket_id" ';
			print ' SIZE="1" VALUE="'.$element_id.'">' ."\n";
			print '</TD></TR>'."\n";
		break;
		case (2): // date. Hidden element
			print '<TR><TD CLASS="td_label" NOWRAP>'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			$date_value = date("Y-m-d H:i:s"); // Only current date in "date added"
			print ' VALUE="'.$date_value .'" >'.$date_value;
			print "</TD> </TR>";  print "\n";
		break;
		case (3): // 3-line inputut field
			print '<TR><TD CLASS="td_label" NOWRAP>'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><TEXTAREA '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print 'COLS="75" ROWS="4"></TEXTAREA>';
			print "</TD> </TR>";  print "\n";
		break;
		case (4): // user. Hidden element
			print '<TR>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="2"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print '  VALUE="'.get_id_from_user($_SESSION['ticket_username']).'">';
			if ($debug)
			{
				print '<br>frm_'. strtolower(mysql_field_name($fields, $i)).'= "'.get_id_from_user($_SESSION['ticket_username']) .'"';
			}
			print "\n";
		break;
		case (5): // action_type. Hidden element
		$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print '  VALUE="'.$GLOBALS['ACTION_DESCRIPTION'].'">';
			print "</TD> ";  print "\n";
		break;
		}
	}
	// Create a Hidden field for the "reason for new $element".
	$reason="New ".$GLOBALS['OPTION']." added to database";
	print '<TD ><INPUT TYPE="hidden" NAME="frm_reason" VALUE="'.$reason.'"></TD>';
	print "</TR></TABLE> \n";
	return 0;
}


/*************************************************************
*** function build_edit_options($id,$table,$reason) *************
*** Builds a form on-the-fly for editing the retrieved data **
*************************************************************/
function build_edit_options($option_id,$reason)
{
	global $debug;
	//For Debugging
	if ($debug)
	{
		print '<p> functions03.inc::build_edit_options($option_id,$table)';
		print '<br>with $option_id = '.$option_id.' and $table = '.$GLOBALS['OPTION_TABLE'] ;
	}
	// Retrieve the field-names of $table from the database $mysql_db_next
	global $link_next ;
	global $mysql_db_next;
	$fields = mysql_list_fields($mysql_db_next, $GLOBALS['OPTION_TABLE'], $link_next);
	$columns = mysql_num_fields($fields);
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $GLOBALS['OPTION_TABLE']); // 0 => short. !0 => long
	// Build the SQL query
	$query = "SELECT * FROM `".$GLOBALS['OPTION_TABLE']."` WHERE `id`='$option_id'";
	if ($debug)
	{
		print '<p> functions03.inc::build_edit_options($option_id,$table) with';
		print '<br> $query= "'. $query .'"<br><br>' ."\n";
	}
	$result = mysql_query($query) or
		do_error('functions03.inc::build_edit_options()::mysql_query()', 'mysql query failed', mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0"  bgcolor="white">';
	//$i=0 is "id", and it should not change ==> TYPE="hidden"
	print '<TR>';
	print '<TD CLASS="td_label">'.$fieldnames[0].'</TD>';
	// Field name first -> $fieldname . Make sure you avoid spaces at begining/end of variable name as " frm_foo" or "frm_foo "
	print '<TD COLSPAN="1"><INPUT TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, 0)).'" ';
	// Then, the form variable -> frm_strtolower($fieldname),
	print ' SIZE="20" VALUE="'.trim($row[mysql_field_name($fields, 0)]).'"> '.$row[mysql_field_name($fields, 0)].'</TD> ';
	// and last, value -> row[$fieldname]
	print '</TR>';	  print "\n";

	if ($debug)
	{
		print '<p> functions03.inc::build_edit_options($option_id,$table)';
		print '<br>Ready to start  building the table' ;
	}
	$tab_value = 1;
	for ($i = 1; $i < $columns; $i++)  // Rest of fields, as Field=0 = ID
	{
		switch ($i)
		{
		case (1):
			print '<TR>';
			print '<TD CLASS="td_label">'.$fieldnames[$i].'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="1"><INPUT '.$value.' TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print ' SIZE="8" VALUE="'.trim($row[mysql_field_name($fields, $i)]).'"></TD> ';
			print '</TR>';	  print "\n";
		break;
		case (2): // Date
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .' (YY-MM-DD)</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="1"><INPUT '.$value.' TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			$this_value = trim($row[mysql_field_name($fields, $i)]);
			print ' SIZE="20" MAXLENGTH="19" VALUE="'.$this_value.'" ></TD> ';  // empty value
			print "</TD> </TR> \n";  print "\n";
		break;
		case (3): // 3-line inputut field
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="1"><TEXTAREA '.$value.' NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print 'COLS="70" ROWS="5" tabindex="'.$i.'" >'.trim($row[mysql_field_name($fields, $i)]).'</TEXTAREA>';
			print "</TD> </TR> \n";  print "\n";
		break;
		case (4): // User
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$this_value = trim($row[mysql_field_name($fields, $i)]);
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="1"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			print ' SIZE="30" VALUE="'.$this_value.'">'.get_owner($this_value).'</TD> ';
			print "</TD> </TR> \n";  print "\n";
		break;
		case (5):
			print '<TR>';
			print '<TD CLASS="td_label">'.$fieldnames[$i].'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="1"><INPUT '.$value.' TYPE="hidden" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			$this_value=get_single_option("action_description"  , trim($row[mysql_field_name($fields, $i)]));
			print ' SIZE="20" VALUE="'.$row[mysql_field_name($fields, $i)].'">'.$this_value.'</TD> ';
			print '</TR>';	  print "\n";
		break;
		default: // Rest of 20-char fields: 2,3,4,6,9,10,11
			print '<TR><TD CLASS="td_label">'.$fieldnames[$i] .'</TD>';
			$value  ='" tabindex="'.$tab_value.'" '  ; $tab_value++;
			print '<TD COLSPAN="1"><INPUT '.$value.' TYPE="text" NAME="frm_'. strtolower(mysql_field_name($fields, $i)).'" ';
			$this_value = trim($row[mysql_field_name($fields, $i)]);
			print ' SIZE="24"  MAXLENGTH="20" VALUE="'.$this_value.'" ></TD> ';  // empty value
			print "</TD> </TR> \n";  print "\n";
		break;
		}
	}
	// Create a Hidden area for the "reason for change".
	$reason="Data has been modified in the database";
	print '<TR> <TD CLASS= td_label > </TD>
			<TD  COLSPAN="1"><INPUT TYPE="hidden" NAME="frm_reason" VALUE="'.$reason.'"></TD>';
	print "</TR></TABLE>\n";
	return 0;
}

/*************************************************************
*** function build_email_option($id) ********************
*** Build Text for email ***********************************
*************************************************************/
function build_email_option($id)
{
	global $debug;
	// Define global variables for data
	global $link_next ;
	global $mysql_db_next;
	global $debug;
	$link_next = mysql_open_next( $mysql_db_next);
	//mysql_open_next("$mysql_db_next", "$mysql_user_next", "$mysql_passwd_next");
	$fields = mysql_list_fields($mysql_db_next, $GLOBALS['OPTION_TABLE'], $link_next); // retrieve list of fields
	$columns = mysql_num_fields($fields);
// Initialize array with obvious wrong values, to detect errors.
	$fieldnames = array("long", "day","at","the", "races");
//Request fildnames, in long version
	$fieldnames = get_fieldname(1, $GLOBALS['OPTION_TABLE']); // 0 => short. !0 => long

	if ($debug)
	{
		print '<p> functions03.inc::build_email__option()';
		print '<br> $id = '.$id . " \n ";
	}
	$email_text = ""; // "<PRE>"; // Zeroing email text content.
	if (1) //if ($id == 0)  // If equals zero, new ticket. Else, existing ticket.
	{
		if ($debug)
		{
			print '<p> functions03.inc::build_email__option()';
			print "<br> \$id is zero \n";
		}
		$frm_temp_value = "frm_" .strtolower(mysql_field_name($fields, 1));
		$email_text = "Adding Action to Ticket#" .trim($_POST[$frm_temp_value] ) ."  \n "  ;	//...  with no extra spaces
		for ($i=2 ; $i < $columns; $i++)  // Generate the queries with a loop. Avoids TicketElement ID ($i=0)
		{
			$frm_temp_value = "frm_" .strtolower(mysql_field_name($fields, $i)); // Build  field values' list
			if ($debug)
			{
				print '<br> $frm_temp_value ='.$frm_temp_value;
				print '<br> $fieldnames[$i] ='.$fieldnames[$i];
			}
			switch ($i) //Build field Values in special way: dates
			{
				case (2): // Action date:
					$temp_text =  "'".date("Y-m-d H:i:s") ."',"     ;	//...  with no extra spaces
				break;
//				case (3): // Problem ens: $frm_problemend
//					$frm_problemend   = $_POST['frm_year_problemend'] .'-'. $_POST['frm_month_problemend'] .'-'. $_POST['frm_day_problemend'] .' ';
//					$frm_problemend   = $frm_problemend   + $_POST['frm_hour_problemend'] .':'.	$_POST['frm_minute_problemend'] .' :00';
//					$temp_text = "'".trim($frm_problemend) ."'"     ;	//...  with no extra spaces
//				break;
				case (4): // Action Owner
					$temp_text = "'".get_owner($_POST[$frm_temp_value])."'";
					//$i=10;
				break;
				case (1): // Ticket Number
//					$temp_text = "'".trim($_POST[$frm_temp_value] )."'"     ;	//...  with no extra spaces
				break;
				/**********
				case (6): // Action Type
				**********/
				case (3):
					$temp_text = "'".trim($_POST[$frm_temp_value] )."'"     ;	//...  with no extra spaces
				break;

			}
// Build email text: Entered values
			$email_text = $email_text  . $fieldnames[$i] . ": $temp_text \n";
		}

	} else {
		if ($debug)
		{
			print '<p> functions03.inc::build_email__option()';
			print "<br> \$id is NOT zero \n";
		}

	}
	//$email_text = $email_text  . "</PRE> \n";
	if ($debug)
	{
		print '<p> functions03.inc::build_email__option()';
		print "<br> \$email_text is: \n ";
		print "<br>$email_text \n ";
	}

	return $email_text;

}


/*************************************************************
*** function edit_option($id,$action,$page,$option) ***
*** Edit the selected option *********************************
*************************************************************/
function edit_option($option_id,$action, $element_id="")
{
	// This fuction is **very specific** of the information in the **table**
	// This means, that it doesn't need to be "generalized".

	// On "case updat", make sure to make the following line works with current fieldnames (i.e.: replace 'frm_description'):
	// elseif ( $_POST['frm_description'] == '') //We need the `description` not empty

	// Define the element and page that use this function,
	// This is useful to easily convert this fucntion to other elements
	// Commented out so uses the data from hipaa. Mmmm. Uncommented as it breaks the options thingy.
	global $portal_user; // $portal_user is the user working in this session.
	global $debug;
	//global $element;
	//$element = $GLOBALS['ELEMENT']	;
	// Build Title Sentence
	$sentence = (($option_id == 0) ? (ucfirst($action)."ing ".$GLOBALS['OPTION']) : (ucfirst($action)."ing ".$GLOBALS['OPTION']." #$option_id"));
	if ($debug )
	 {
		 print '<p> functions03::inc.php::edit_option()';
		 print '<br> $option_id =  <b>'.$option_id.'</b>';
		 print '<br> $_GET[action] = <b>'.$_GET['action'].'</b>';
		 print '<br> $action = <b>'.$action.'</b>';
		 print '<br> $element_id = <b>'.$element_id.'</b>';
		 print '<br>$portal_user = <b>'.$portal_user .'</b><br><br>' . "\n";
		 print '<br>$sentence = <b>'.$sentence.' </b>';
	 }
	$temp_trailing = ($option_id != 0 )? (" #$option_id") : (" "); // Whichen adding, $option_id is *not* really zero
	//set_htmlheader(ucfirst($action)."ing ".$GLOBALS['OPTION']." " . $temp_trailing);
	do_title(ucfirst($action)."ing ".$GLOBALS['OPTION']." " . $temp_trailing, 0); // Begin the webpage. "0" means do not insert headers.
	// Debug mode.
	switch($action)  // Which course of action should we take?
	{
		// Adding new $GLOBALS['OPTION']
		case "add":
		if ($debug )  print '<p>functions03.inc::edit_option(action=add)<br>$option_id='.$option_id .'<br>';

		if ($option_id != 0 ) // sanity check . We don't want to mess existing records (existing record: id != 0 )
		{
			if ($debug )
			{
				print '<p>functions03.inc::edit_option()::action=add id!=0';
				print '<br>Inside invalid id (it should be zero for adding) -- $option_id = ' .$option_id;
				print '<br>$_GET[action] = '.$_GET['action'];
				print '<br>$portal_user = '.$portal_user . " \n ";
			}
			print "<P CLASS=\"warn\">Invalid ".$GLOBALS['OPTION']." ID: '$option_id' . I cannot add. Exiting.</P><BR> \n";
			return;
		}
		else
		{
			if ($debug )
			{
				print '<p>functions03.inc::edit_option()::action=add id=0';
				print '<br> $option_id = ' .$option_id ;
				print '<br>$_GET[action] = '.$_GET['action'];
				print '<br>$portal_user = '.$portal_user . " \n ";
			}
			print '<table border="0" cellpadding="2" cellspacing="2" ><tr><td valign="top" >';	  print "\n";
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['SECOND_PAGE'].'.php?id=0&action=updat">';
			build_add_options(0,$element_id);
			print '<BR><INPUT TYPE="submit" VALUE="Submit '.$GLOBALS['OPTION'].'"></FORM><br>';		  print "\n";
			print "</td><td valign=\"top\">";
			print '<P CLASS="severity_medium"> * Use TAB key to move around fields</P>';
			print '<P CLASS="severity_medium"> * Please enter a description of the action taken</P>';
			print "\n";
			print "</td></tr></table>";	  print "\n";
			//list_option();
			print "\n"; mysql_shutdown_next();
		}
		break;

		case "edit":  // Editing
		if ($debug )
		{
			print '<p> functions03.inc::edit_option(edit)';
			print '<br>$option_id = '.$option_id;
			print '<br>$_GET[action] = '.$_GET['action']." \n ";
		}
		if ($option_id == '' OR $option_id <= 0)// OR !check_for_rows("SELECT * FROM $GLOBALS['OPTION_TABLE'] WHERE id='$option_id'"))
		{
			if ($debug )
			{
				print '<p> functions03.inc::edit_option(edit)::Warning ';
				print '<br> $option_id = ' .$option_id ;
				print '<br> $_GET[action] = '	.$_GET['action'];
				print '<br> $portal_user = '.$portal_user ." \n ";
			}
			print "<FONT CLASS=\"warn\">Invalid ".$GLOBALS['OPTION']." ID: '$option_id'</FONT><BR>";
		}
		else
		{
			$query = "SELECT * FROM ".$GLOBALS['OPTION_TABLE']." WHERE id='$option_id'";
			if ($debug)
			{
				print '<p>functions03.inc::edit_option(edit/updat)';
				print '<br>$option_id = ' .$option_id;
				print '<br>$_GET[action] = '.$_GET['action'];
				print '<br> $query = '.$query;
				print '<br>$portal_user = '.$portal_user;
				print '<br>$GLOBALS[MAIN_PAGE]= '.$GLOBALS['MAIN_PAGE'] ." \n ";
			}
			$result = mysql_query($query) or
				do_error('functions03.inc::edit_option:updat', 'mysql_query() failed', mysql_error());
			$row = mysql_fetch_array($result);
			// Show the current information in editable fields from the browser.
			// form name = 'frm' + '_' + 'lowercase(fieldname)'
			print '<table  bgcolor="white"><tr><td valign="top" width="20%"><tr><td>';	  print "\n";
			//add_element_buttons($option_id, $GLOBALS['SECOND_PAGE']	, "$option" . "s" );
			add_action_buttons($option_id, $row['ticket_id']);
			print '</td></tr><tr><td valign="top" width="80%">';  print "\n";
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['SECOND_PAGE'].'.php?id='.$option_id.'&action=updat">'; print "\n";
			$reason = "";
			build_edit_options($option_id,$reason); print "\n";
			print'<BR><INPUT TYPE="submit" VALUE="Submit Changes"></FORM><br>';		print "\n";
			//list_option("");
			//mysql_shutdown_next();
			print '</td></tr></table>';
		}
		break;

		case "updat": 		// Updating
		if ($debug )
		{
			print '<p> functions03.inc::edit_option(update)';
			print '<br>$option_id = "' .$option_id .'"';
			print '<br>$_GET[action] = "' .$_GET['action'] .'"';
			print '<br>$portal_user = "'.$portal_user .'"';
			print '<br>$GLOBALS[OPTION_TABLE]= "'.$GLOBALS['OPTION_TABLE'] . " \n ";
		}
		if ( $_POST['frm_reason'] == '')
		{
			print " \n <P><FONT CLASS=\"warn\">functions03.inc::edit_option(update):";
			print "<BR>No reason given for the update of ".$GLOBALS['OPTION'].": '$option_id'. <BR>Try again.</FONT>"; print "\n";
			edit_option($option_id, "edit");  print "\n";
		}
		elseif ( $_POST['frm_description'] == '') //We need the `description` not empty
		{
			print "<p><FONT CLASS=\"warn\">functions03.inc::edit_option(update):";
			print "<BR>Missing 'value' field for the update of ".$GLOBALS['OPTION'].": '$option_id'.";
			print "<BR>Try again.</FONT>"; print "\n";
			edit_option($option_id, "edit");  print "\n";
		}
		elseif ($option_id == 0)
		{
			//Insert $Element
			$query = make_insert_query_action( "insert",$option_id); // Call make_insert_query() to build the quey.
			$result = mysql_query($query) or
				do_error('functions03.inc::edit_option:update', 'mysql_query() failed', mysql_error);
			// Time to do Add New Element.
			print '<table><tr><td width="30%">';
			print '<p  CLASS="severity_medium">New '. $GLOBALS['OPTION']	. ' has been added.  ';
			print '</td></tr></table>'; print "\n";
			print '<br><br><br>';
			print '<table width="300"><tr>';
			print '<td width="50%" align="left">';
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php?id='.$_POST['frm_ticket_id'].'">' . " \n";
			print'<INPUT TYPE="submit" VALUE="Go Back to Ticket &nbsp; #'.$_POST['frm_ticket_id'].'"></FORM></td>'. " \n";
			print '<td width="50%" align="right">';
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php?id='.$_POST['frm_ticket_id'].'&action=close">' . "\n";
			print'<INPUT TYPE="submit" VALUE="Close '.$GLOBALS['ELEMENT']	. ' &nbsp; #'.$_POST['frm_ticket_id'] .' "></FORM> </td>';		print "\n";
			print '</tr></table>';

			// Insert the reason with Transaction Management: log user, date, time, changes.
			if ($debug)
			{
				 /***
				 print '<p class="grey">From user:<b>'.$portal_user.'</b> , on '. date("H:i, jS F, Y").
				'<br> <font class="severity_medium">Inserting the reason:</font> '.$_POST['frm_reason'] .'<BR><BR>';   print "\n";
				transaction_management($option_id, $GLOBALS['OPTION_TABLE'], $action,$_POST['frm_reason'], $portal_user);   print "\n";
				***/
				print '<p>functions03.inc::edit_option(updat, id=0)';
				print '<br>About to do show_data($_POST[frm_ticket_id],$GLOBALS[MAIN_TABLE],$GLOBALS[ELEMENT],$GLOBALS[SECOND_PAGE],$start_id)';
				print '<br>$_POST[frm_ticket_id]='. $_POST['frm_ticket_id'];
				print '<br>$GLOBALS[MAIN_TABLE]='.$GLOBALS['MAIN_TABLE'];
				print '<br>$GLOBALS[ELEMENT]	='.$GLOBALS['ELEMENT']	;
				print '<br>$GLOBALS[SECOND_PAGE]	='.$GLOBALS['SECOND_PAGE']	;
				//print '<br>$start_id='.$start_id  ." \n ";
			}
			mysql_shutdown_next();

			if ($GLOBALS['SEND_EMAIL_OPTION'])
			{
				// Send email with text of action
				$email_text = build_email_option($option_id);
				send_email($email_text);
			}
			return 0; // All should be allright now.
		}else
		{
			// update $GLOBALS['OPTION']
			if ($option_id == '' OR $option_id < 0)// OR !check_for_rows("SELECT * FROM $GLOBALS['OPTION_TABLE'] WHERE id='$option_id'") )
			{
				if ($debug )
				{
					print '<p>functions03.inc::edit_option(update)';
					print '<br> Invalid id. $option_id = ' .$option_id ;
					print '<br>$_GET[action] = '.$_GET['action'];
					print '<br> $portal_user = '.$portal_user ." \n ";
				}
				print "<FONT CLASS=\"warn\">Invalid ".$GLOBALS['OPTION'].": '$option_id' </FONT>";   print "\n";
				//if (trim($GLOBALS['OPTION'])=="") { $GLOBALS['OPTION'] = trim($GLOBALS['OPTION_TABLE']) ;}
				show_data($option_id,$GLOBALS['MAIN_TABLE'],$GLOBALS['ELEMENT'],$GLOBALS['SECOND_PAGE']); // ($option_id,$GLOBALS['OPTION_TABLE'],$title,$GLOBALS['SECOND_PAGE']	)   print "\n";
			} else
			{
				// Time to do updates.
				print '<FONT CLASS="severity_medium">'.$GLOBALS['OPTION'].' has been updated</FONT><BR><BR>';  print "\n";
				// Insert the reason with Transaction Management: log user, date, time, changes.
				 /******************
				print '<p class="grey">From user:<b>'.$portal_user.'</b> , on '. date("H:i, jS F, Y").
				'<br> <font class="severity_medium">Inserting the reason:</font> '.$_POST['frm_reason'] .'<BR><BR>';   print "\n";

				//transaction_management($option_id, $GLOBALS['OPTION_TABLE'], $action,$_POST['frm_reason'], $portal_user);  print "\n";

				print '<p class="severity_medium"> Inserting the reason:<br><font class="grey">From user:<b>'.$portal_user.'</b> ,
						on '. date("H:i, jS F, Y").'<br>'.$_POST['frm_reason'] .'</font><BR><BR>';   print "\n";
				transaction_management($option_id, $GLOBALS['OPTION_TABLE'], $action,$_POST['frm_reason'],$portal_user);		  print "\n";
				******************/
				//Update $Element function make_insert_query($GLOBALS['OPTION_TABLE'], $type,$option_id,$visit=0,$aid=0)
				$query=make_insert_query($GLOBALS['OPTION_TABLE'], "update", $option_id);
				// On error, exit.
				$result = mysql_query($query) or do_error('functions03::edit_option(updat)::mysql_query()', 'mysql query failed', mysql_error());
				// show updated $GLOBALS['OPTION']
				//show_action_data($option_id, $GLOBALS['OPTION_TABLE'],$GLOBALS['SECOND_PAGE'],$GLOBALS['MAIN_PAGE']); // ($option_id,$GLOBALS['OPTION_TABLE'],$title,$GLOBALS['SECOND_PAGE']	)   print "\n";
			}
		}
		break;

		// Deleting
		case "delet":
		if ($debug )
		{
			print '<p>functions03.inc::edit_option(delete)';
			print '<br>$_POST[frm_confirm] = ' . $_POST['frm_confirm'] ;
		}
		if ($_POST['frm_confirm'])
		{
			if ( $_POST['frm_reason'] == '')
			{
				print "<P CLASS=\"warn\">functions03.inc::edit_option(delete): No reason given for removal of ".$GLOBALS['OPTION'].": '$option_id'. <BR>Try again.</P>";  print "\n";
				show_data($option_id,$GLOBALS['MAIN_TABLE'], $GLOBALS['ELEMENT'],$GLOBALS['SECOND_PAGE']);	  print "\n";
			}else
			{
				if ($debug )
				{
					print '<p>functions03.inc::edit_option(delete)';
					print '<br>$option_id = ' .$option_id ;
					print '<br> $_GET[\'action\'] = '.$_GET['action'];
					print '<br>$portal_user = '.$portal_user ;
					print '<br>$_POST[\'frm_confirm\'] = '.$_POST['frm_confirm']."\n";
				}
				// remove $ELEMENT and make sure its logged into the database
				//transaction_management($option_id, $GLOBALS['OPTION_TABLE'], $action,$_POST['frm_reason'], $portal_user);  print "\n";
				$result = mysql_query("DELETE FROM ".$GLOBALS['OPTION_TABLE']." WHERE ID='$option_id'")
					or do_error('functions03.inc::edit_option(delete)', 'mysql_query() failed', mysql_error());
				print "<P CLASS=\"header\">".$GLOBALS['OPTION']." number '$option_id' has been removed.</P><BR><BR>";	  print "\n";
				list_option();	  print "\n";
			}
		}
		else
		//confirm deletion
		{
			$query = "SELECT * FROM ".$GLOBALS['OPTION_TABLE']." WHERE id='$option_id'";
			if ($debug )
			{
				print '<p>functions03.inc::edit_option(delet)';
				print '<br>$option_id = ' .$option_id ;
				print '<br>$_GET[action] = ' .$_GET['action'] ;
				print '<br>$query = '.$query;
				print '<br>$portal_user = '.$portal_user ."\n";
			}
			$result = mysql_query($query) or
				do_error('functions03.inc::edit_option:delet', 'mysql_query() failed', mysql_error());
			$row = mysql_fetch_array($result);

			if ($debug )
			{
				print '<p>functions03.inc::edit_option(delete)';
				print '<br>$option_id = ' .$option_id;
				print '<br>$_GET[action] = '.$_GET['action'];
				print '<br>$portal_user = '.$portal_user;
				print '<br>$_POST[frm_confirm] = '.$_POST['frm_confirm'] ." \n ";
			}

			print "<FONT CLASS=\"severity_medium\">Confirm deletion of ".$GLOBALS['OPTION']." #$option_id ";
			print "<br>\"$row[2], $row[3], $row[4], $row[5], \" </FONT>";
			print '<BR><BR><FORM METHOD="post" ACTION="'.$GLOBALS['SECOND_PAGE'].'.php?id='.$option_id.'&action=delet&go=1">' . "\n \n";
			print "<TABLE BORDER=\"0\">";
			print "<TR><TD CLASS=\"td_greylabel\">Delete ".$GLOBALS['OPTION']." #$option_id ?&nbsp;</TD>
				<TD><INPUT TYPE=\"checkbox\" NAME=\"frm_confirm\" VALUE=\"1\">&nbsp;&nbsp;Yes, delete this ".$GLOBALS['OPTION']."</TD>
				</TR> \n";
			print "<TR>
				<TD CLASS=\"td_greylabel\">Provide a Reason for<br>deleting the ".$GLOBALS['OPTION'].":</TD>
				<TD><TEXTAREA NAME=\"frm_reason\" COLS=\"40\" ROWS=\"4\">$reason</TEXTAREA></TD></TR>\n \n";
			print "</TABLE><br> \n";
			print "<INPUT TYPE=\"Submit\" VALUE=\"Confirm\">";
			print "</FORM><br> \n";
			print '<FORM METHOD="post" ACTION="action.php"><INPUT TYPE="submit" VALUE="Cancel"></FORM> ' . "\n";
		}
		break;

		// Showing info, so we can edit the data.
		case " ":
			print  '<P CLASS="warn"> No action given. Stop.</p>';	  print "\n";
		break;
	}
}


/***********************************************************************
*** function list_option($table,$SQL=" WHERE 1 ",$current=0,$search=0) ***
*** Displays the selected table, sortable ASC and DESC by  *************
*** any of the table's fields. *****************************************
*** $search means this is a the result of user's search ****************
***********************************************************************/
function list_option($SQL="",$current=0,$search=0)
{
	// $table : table to use. It will always be $GLOBALS['OPTION_TABLE']
	// $SQL : extra SQL code, like searches. Defaults as " WHERE 1 " when no extra SQL needed
	// $current is current position in shown results  (defaults to first element)
	// $search : is this a search? 0 = no / 1 = yes
	global $debug;
	global $per_page;
	if ($per_page == 0)  $per_page = 20;  // Limit results shown to 20 results per page
	$ident = 0;
	$page="action.php";
	// On error, exit.
	//if (! $table ) 	{ print "<P class=\"severity_medium\"> Error! no variable given in  list_option(\$table)!"; return 0; }
	//Ternary operator, sort order, ascending/descending
	if  ($debug) // For Debug
	{
		print " \n <p>functions03.inc::list_option()";
		print '<br>pre $table is '.$GLOBALS['OPTION_TABLE'] ;
		print '<br>pre $_GET[order] is '.$_GET['order'] ;
		print '<br>pre $_GET[sortby] is '.$_GET['sortby'];
		print '<br>pre $_GET[ident] is '.$_GET['ident'] ;
		print '<br>$serarch is "'.$search. "\" \n ";
	}
	$order = ($_GET['order'] == ('')) || ($_GET['order'] == ('DESC'))  ? 'DESC' : 'ASC';
	// Ternary operator that decides sort order on click.
	$sortby = $_GET['sortby'] == '' ? 'id ' : $_GET['sortby'] ;
	// Get current starting threshold. If there's none, equals zero.
	$current = $_GET['current'] == '' ? 0 : $_GET['current'];
	// SQL query --> How many rows do we have in total?
	$query_max = "SELECT `id` FROM `".$GLOBALS['OPTION_TABLE']."`  WHERE 1 ".$SQL . " " ;  // Select ids from table (fast query)
	if ($debug) // For Debug
	{
		print " \n <p>functions03.inc::list_option()";
		print '<br>post $order is '.$order;
		print '<br>post $sortby is '.$sortby;
		print '<br>post $ident is '.$ident;
		print '<br>$query_max(maxnum) = '.$query_max." \n ";
	}
	$result_max = mysql_query($query_max) or
		do_error('functions03.inc::list_option()::mysql_query()', 'mysql query failed with $query_max '. $query_max, mysql_error());
	$maxrows=mysql_num_rows($result_max);

	// Build real SQL query
	$query = "SELECT * FROM `".$GLOBALS['OPTION_TABLE']."`  WHERE 1 ". $SQL. " ORDER BY $sortby $order LIMIT ".$current.",$per_page";

	$result = mysql_query($query) or
			do_error('functions03.inc::list_option()::mysql_query()', 'mysql query failed with $query '. $query, mysql_error());

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
		print " \n <p>functions03.inc::list_option()";
		print '<br>$query = '.$query ;
		print '<br>I\'m out: Starting generation of elements\' table'."   \n ";
	}
	$index_of_pages=index_elements($maxrows,$current,$per_page,$order,$SQL);
	// (previous line) Index of elements shown. Show $per_page elements at a time
	print '<table  width="750" border="0">'; // Start Results table.

// Search if there is a limited number of fields to show OR if there is special order of fields to be shown.
	global $table_fields;
	which_fields($GLOBALS['OPTION_TABLE']); // filling $table_fields

if (sizeof($table_fields) == 0) // Should we display all fields? or only a subset? If sizeof()== 0, All fields
{
	if ($debug)
	{
		print '<h3>There is NO stuff in which_fields()</h3>';
		print '<p>functions03.inc::list_option(). "table" is: ' . $GLOBALS['OPTION_TABLE'] ." \n ";
	}
	if ($row = mysql_fetch_array($result)) // As we use this fetch_array, we will use do{} instead of while{}
	{
		// Switch between ASC and DESC sort order.
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
			print " \n <p>functions03.inc::list_option(): inside table generation. row=mysql_fetch_array(result). Print all fields";
			print'<br>$order="'. $order .'"' ;
			print'<br>number of fields is $iflds="'. $iflds .'"' ;
			print'<br>number of rows is $irows="'. $irows  .'"';
			print'<br>$table="'. $GLOBALS['OPTION_TABLE']  ."\" \n ";
		}
		$fieldnames = get_fieldname(0, $GLOBALS['OPTION_TABLE']); // 0 => short. !0 => long
		print "  <TR>";  // print row for titles
		for ($i = 0; $i <= $iflds; $i++)
		{
			// This is the field name.
			$ident = (mysql_field_name($result, $i));
			if($debug)
			{
				print " \n <p>functions03.inc::list_option():: Headers. in for()...switch()";
				print '<br> $i="'.$i.'"';
				print '<br> $ident="'.$ident."\" \n";
			}
			// Print headers with a link to field name, for sorting pourposes.
			switch ($i)
			{
			case "0": // Short Fields: id
				print '<TH class="thead">&nbsp;&nbsp;'.$fieldnames[$i].'&nbsp;&nbsp;</TH>'."\n";
			break;
			default: // Other fields
				print '<TH class="thead" NOWRAP>'.$fieldnames[$i].'</TH>'."\n";
			break;
			}
		}
		if ($debug)
		{
			print " \n <p>functions03.inc::list_option(): Just finished for() switch()...";
			print ' <br>And $i was "'.$i.'" in the end of it';
		}
		print " </TR> \n"; // End of titles' row
		// We already have fetched an array of data. Let's do "do{...}while()" instead of "while(){...}"
	}  else
	{
		// Thanks to 2metre <nws@hersham.net> from news:comp.lang.php for --> " $iflds = mysql_num_fields($result) - 1; "
		$iflds = mysql_num_fields($result) - 1;		// Number of fields
		$irows = mysql_num_rows($result); // number of rows
		if($debug)
		{
			print " \n <p>functions03.inc::list_option() -- I'm in the else";
			print '<br>number of fields is '. $iflds ;
			print '<br>number of rows is '. $irows ;
		}
		print '<p class="severity_medium"> No results were found (Num of rows is '.$irows.' )';
	}
} else { // If there is only a subset of fields to show.
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
			print " \n <p>functions03.inc::list_option(): inside table generation. row=mysql_fetch_array(result). Print SOME fields";
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
				print " \n <p>functions03.inc::list_option():: Headers. in while{ switch () }";
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
			print '<TR >';
			while( $element_data = each($table_fields)) // [1] contains the proper 'value'. [0] contains 'order'
			{
				if ($debug)
				{
					print "<br>current element_data is $element_data[1]";
				}
				switch ($element_data[1])
				{
				case "0": // Short Field: Num
					print '<TD class="'.$tblcolor.'"  NOWRAP style="width: 15% "><A HREF="'.$page.'?id='.$row[0].'&action=edit">#'.$row[0].'</a></TD>'."\n";
				break;
				case (1): // ticket_id
					print '<TD class="'.$tblcolor.'" NOWRAP style="width: 15% ">'.$row[1];
					print '</TD>'."\n";
				break;
				case (2): // date.
					print '<TR><TD class="'.$tblcolor.'"  NOWRAP style="width: 15% "><A HREF="'.$page.'?id='.$row[0].'&action=edit">'.$row[2];
					print '</a></TD>'."\n";
				break;
				case (3): // 2-line inputut field
					print '<TD class="'.$tblcolor.'" WRAP><pre
						style=" white-space: pre-wrap; /* css-3 */ white-space: -moz-pre-wrap; /* Mozilla, since 1999 */
						white-space: -pre-wrap; /* Opera 4-6 */ white-space: -o-pre-wrap;    /* Opera 7 */ word-wrap: break-word; ">'.$row[$element_data[1]];
					print '</pre></TD>'."\n";
				break;
				case (4): // user. Hidden element
					print '<TD class="'.$tblcolor.'" NOWRAP style="width: 10% ">'.get_owner($row[$element_data[1]]);
					print '</a></TD>'."\n";
				break;
				case (5): // action_type. Hidden element
					print '<TD class="'.$tblcolor.'" NOWRAP>'.get_single_option("action_description",$row[$element_data[1]]);
					print '</a></TD>'."\n";
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
			print " \n ".'<p>functions03.inc::list_option() || I\'m in the else<br>';
			print '<br>number of fields is '. $iflds ;
			print '<br>number of rows is '. $irows ;
		}
		print '<p class="severity_medium"> No results were found (Num of rows is '.$irows.' )';
	}
}

	print "</table>\n <br>"; // Finish table
	print $index_of_pages; // This substitutes previous line. Hopefuly, now index with proper ASC/DESC will work.
	if($debug)
	{
		print'<p>functions03.inc::list_option(): Exiting function, with $high as '. $high;
	}
	return $high; //Yeah, we will be high after all this code.
}


/*******************************************************************
*** function make_insert_query_action() **********************
*** Build the query for the $action = 'updat' case *****************
*******************************************************************/
function make_insert_query_action($type,$id)
{
	// On error, exit .
	if (! $type)
	{
		print '<P class="severity_medium"> Error! no variable given to make_insert_query_action($type)!';
		return 0;
	}
	// Define global variables for data
	global $link_next ;
	global $mysql_db_next;
	global $debug;
	$fields = mysql_list_fields($mysql_db_next, $GLOBALS['OPTION_TABLE'], $link_next); // retrieve list of fields
	$columns = mysql_num_fields($fields);
	if ($debug)
	{
		print '<p> functions03.inc::make_insert_query_action()';
		print '<br>$table ='.$GLOBALS['OPTION_TABLE'];
		print '<br>$type='.$type;
		print '<br>$id='.$id;
	}
	switch ($type)
	{
	case "insert": // Insert $element
		$i = 1; // We don't want to put the id value (auto fileld/increment field)
		$query = "INSERT INTO ".$GLOBALS['OPTION_TABLE']." ("; // Start the query for INSERT
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
				case (2): // Ticket date: t_date
					$temp_query = $temp_query . "'".date("Y-m-d H:i:s") ."',"     ;	//...  with no extra spaces
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
		$i = 1 ; // We want to leave alone the id value (it's the zero element)
		// Dinamicaly build field and frm_* names
		$query = "SELECT * FROM ".$GLOBALS['OPTION_TABLE']." WHERE ID='$id'";
		$result = mysql_query($query) or
					do_error('functions03.inc::make_insert_query_action()::mysql_query()', 'mysql query failed', mysql_error());
		$row = mysql_fetch_array($result);
		$query = "UPDATE ".$GLOBALS['OPTION_TABLE']." SET ";
		for ($i ; $i < $columns; $i++)
		{
			$query = $query ."`".trim(mysql_field_name($fields, $i)). "`='"  ; // Build field names' list with no extra spaces
			$frm_temp_value = "frm_" .strtolower(mysql_field_name($fields, $i)); // Build  field values' list
			$query = $query . trim($_POST[$frm_temp_value] )."'"     ; // ... with no extra spaces
			if ($i < ($columns-1)) $query =  $query. "," ; // Finish up the query.
		}
		$query = $query ." WHERE id='".$_POST[frm_id]."'"; // This '$_POST....' is the table id, not '$id'

		if ($debug)
		{
			print "<p>The dinamically build \$query on functions03.inc::make_insert_query_action()::update<br>";
			print $query."\n";
		}
		break;
	}
	if ($debug)
	{
		print '<p>Inside functions03.inc::function make_insert_query_action()::'.$type;
		print '<br>$query is :<br> '.$query."<br> \n";
	}
	return $query;
}



/*************************************************************
***  function show_action_data($id,$table,$name,$page,$start_id) ****
*** Show the selected $element, building all dinamicaly ******
*************************************************************/
function show_action_data($id,$table,$start_id=0)
{
	global $debug;
	// Begin the webpage
	do_title("Showing ".$GLOBALS['OPTION']." #$id");
	//For Debugging
	if ($debug)
	{
		print " \n <p>functions03.inc::show_action_data(): Showing ".$GLOBALS['OPTION']." $id \n";
	}
	/* sanity check */
	if ($id == '' OR $id <= 0 OR !check_for_rows("SELECT * FROM $table WHERE id='$id'"))
	{
		print "<p>functions03.inc::show_action_data()<br>Invalid ".$GLOBALS['OPTION']." ID: '$id'<br>\n";
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
	$query = "SELECT * FROM $table WHERE ID='$id'";
	if ($debug)
	{
		print " \n <br>show_action_data(): query is '$query' \n";
	}
	$result = mysql_query($query) or
		do_error('functions03.inc::show_action_data():mysql_query()', 'mysql query failed', mysql_error());
	$row = mysql_fetch_array($result);
	print '<TABLE BORDER="0" bgcolor="white"> <tr><td  width="25%" valign="top">'; 	  print "\n";
	add_action_buttons($id, $row['ticket_id'] );
	print '</td></tr><tr><td  valign="top" width="70%">';	  print "\n";
	print '<TABLE BORDER="0" bgcolor="white" width="100%">';
	print '<thead><p class="severity_medium">'.$GLOBALS['OPTION'].'\'s Information</p></thead>';
	// $start_id used to hide all index fields
	for ($i = ($start_id+1); $i < $columns; $i++)  // i=0 --> id. Let's hide this field.
	{
		print '<TR>';
		print ' <TD CLASS="td_greylabel" width="25%">'.$fieldnames[$i].':';
		if ($debug)
		{
			print " \n <br>show_action_data(): \$i is '$i' \n";
			print " \n <br>show_action_data(): \$fieldnames[\$i] is '$fieldnames[$i]' \n";
			print " \n <br>show_action_data(): \$row[\$i] is '$row[$i]' \n";
		}
		print '</TD>';  print "\n";
		print ' <TD width="75%">';
		switch ($i)
		{
				case "0": // Short Field: Num
				case (1): // ticket_id
				case (2): // date.
				case (3): // 2-line input field
					print $row[$i];
				break;
				case (4): // user. Hidden element
					print  get_owner($row[$i]);
				break;
				case (5): // action_type. Hidden element
					print get_single_option("action_description",$row[$i]);
					print '</a></TD>'."\n";
				break;
		}
		print ' </TD></TR>';	  print "\n";
	}
	print '</TABLE>';	 print "\n";

	print '</td></tr></TABLE>';	  print "\n";

	mysql_shutdown_next();
	return 0;
}



/***********************************************************************
*** function do_something_option() *************************************
*** Work needs to be done  *********************************************
***********************************************************************/
function do_something_option()
{
	print '<p>Nothing here -yet-';
	powered("1");
}

?>
