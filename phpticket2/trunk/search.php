<?php 
/**************************************   
*** File: search.php   *****************
Project: ticket2 (phpTicket New Generation)
***************************************
*** Author: Sinner from the Prairy ***
*** email: sinnerbofh@gmail.com *****
*** Comment: phpTicket New Generation, based on  ticket.sf.net*
**************************************/

// Activate 'action' search
$action_is_activated=0;
$single_search_is_activated=1;
$search_help_is_activated=0;

require_once('conf.inc');
require_once('functions01.inc');
require_once('functions02.inc');

$page="search"; // name of this current php page. Use 'index' for 'index.php'.
do_login($page .'.php');

global $debug;
$debug = get_variable("debug_value"); 
//$_SESSION['ticket_username']; = $_SESSION['ticket_username'];
$option_table = $GLOBALS['OPTION_TABLE'];  //"action";
$linking_field = "ticket_id";

global $mysql_db_next;
$link_next = mysql_open_next( $mysql_db_next); // Open second Database on MySQL Server


if ($debug) // (1) // 
{
	print " \n ".'<p>'.$page.'.php::<br>';
	print '<br> $_GET[term] ="'.$_GET['term'] ."\" \n ";
	print '<br> $_POST[frm_query] ="'.$_POST['frm_query'] ."\" \n ";
	print '<br> $_POST[frm_owner] ="'.$_POST['frm_owner'] ."\" \n ";
	print '<br> $_POST[frm_querytype] ="'.$_POST['frm_querytype'] ."\" \n ";
	print '<br> $_POST[frm_search_in] ="'.$_POST['frm_search_in'] ."\" \n ";
	print '<br> $_SESSION[ticket_username] ="'.$_SESSION['ticket_username'] ."\" \n ";
	
}
 
switch ($_GET['term'])
{
	case (""):
		do_title("Search Page");
		print '<FORM METHOD="post" ACTION="search.php?term=ticket">' ."\n";
		print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
		// Ticket search
		print '<TR><TD VALIGN="top">';
		print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
		print '<THEAD><TR ALIGN="LEFT" ><TD COLSPAN="2" CLASS="td_link"> Enter Tickets\' Search Terms </TD></TR></THEAD>' ."\n";
		print '<TR><TD VALIGN="top" CLASS="td_label">Query: &nbsp;</TD>' ."\n";
		print '<TD><INPUT TYPE="text" SIZE="40" MAXLENGTH="255" VALUE="'.$_POST['frm_query'].'" NAME="frm_query"></TD></TR>' ."\n";
		print '<TR><TD VALIGN="top" CLASS="td_label">Search in: &nbsp;</TD>' ."\n";
		print '<TD><SELECT NAME="frm_search_in">' ."\n";
		print '<OPTION VALUE="" checked>All</OPTION>' ."\n";
		print '<OPTION VALUE="description">Description</OPTION>' ."\n";
		print '<OPTION VALUE="affected">Affected</OPTION>' ."\n";
		print '<OPTION VALUE="scope">Type</OPTION>' ."\n";
		print '<OPTION VALUE="owner">Owner</OPTION>' ."\n";
		print '<OPTION VALUE="t_date">Issue Date</OPTION>' ."\n";
		print '<OPTION VALUE="problemstart">Problem Starts</OPTION>' ."\n";
		print '<OPTION VALUE="problemend">Problem Ends</OPTION>' ."\n";
		print '</SELECT></TD></TR>' ."\n";
		// Search by Location
			print '<TR><TD VALIGN="top" CLASS="td_label">Location: &nbsp;</TD>' ."\n";
			print '<TD><SELECT NAME="frm_location">' ."\n";
			print '<OPTION VALUE="" checked>All</OPTION>' ."\n";
			get_option_constant("location"  , "All"); //, $index);
			print '</SELECT></TD></TR>' ."\n";
		// Search by Owner	
			print '<TR><TD VALIGN="top" CLASS="td_label">Owned by: &nbsp;</TD>' ."\n";
			print '<TD><SELECT NAME="frm_owner">' ."\n";
			print '<OPTION VALUE="" checked>All</OPTION>' ."\n";
			//get_option_owner("user", $_SESSION['ticket_username']); // , $index);
			get_option_owner( get_id_from_user($_SESSION['ticket_username'])); // , $index);
			print '</SELECT></TD></TR>' ."\n";
		// Search Open , Closed, All.
		print '<TR><TD VALIGN="top" CLASS="td_label">Status: &nbsp;</TD>' ."\n";
		print '<TD><INPUT TYPE="radio" NAME="frm_querytype" VALUE="%" CHECKED> All &nbsp;' ."\n";
		print '<INPUT TYPE="radio" NAME="frm_querytype" VALUE="'.$STATUS_OPEN.'"> Open &nbsp;' ."\n";
		print '<INPUT TYPE="radio" NAME="frm_querytype" VALUE="'.$STATUS_CLOSED.'"> Closed' ."\n";
		print '</TD></TR>' ."\n";
		print '<TR><TD COLSPAN="2"> <hr></TD></TR>' ."\n";
		// Order  Search result By, ASC or DESC
		print '<TR ALIGN="LEFT" ><TD COLSPAN="2" CLASS="td_link">Order By: &nbsp;</TD></TR>' ."\n";
		print '<TR>' ."\n";
		print '<TD><SELECT NAME="frm_ordertype">' ."\n";
		print '<OPTION VALUE="t_date">Issue Date</OPTION>' ."\n";
		print '<OPTION VALUE="problemstart">Problem Starts</OPTION>' ."\n";
		print '<OPTION VALUE="problemend">Problem Ends</OPTION>' ."\n";
		print '<OPTION VALUE="affected">Affected</OPTION>' ."\n";
		print '<OPTION VALUE="scope">Type</OPTION>' ."\n";
		print '<OPTION VALUE="owner">Owner</OPTION>' ."\n";
		print '</SELECT></TD>' ."\n";
		print '<TD>&nbsp;Sort Result in <b>Descending</b> Order: <INPUT TYPE="checkbox" NAME="frm_order_desc" VALUE="DESC" CHECKED>';
		print '<br>(Uncheck for <b>Ascending</b> sort order)</TD></TR>' ."\n";

		print '<TR><TD COLSPAN="2"> <hr></TD></TR>' ."\n";
		
		print '<TR><TD></TD>' ."\n";
		print '<TD><INPUT TYPE="submit" VALUE="Search in Tickets"></TD></TR>' ."\n";
		print '</TABLE></FORM>' ."\n";
		print '</TD>';
		if ($single_search_is_activated)
		{
		
			// Search for one single element number
			print '<TD VALIGN="top"> ';
			print '<FORM METHOD="post" ACTION="'.$GLOBALS['MAIN_PAGE'].'.php">' ."\n";
			print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
			print '<TR><TD VALIGN="top" CLASS="td_label"  COLSPAN="2">To search by '.$GLOBALS['ELEMENT'].' number</TD></TR>' . " \n ";
			print '<TR><TD VALIGN="top"  COLSPAN="2">Enter the desired '.$GLOBALS['ELEMENT'].' number and click "Go"</TD><TR>' . " \n ";
			print '<TR><TD VALIGN="top"  COLSPAN="2"> <hr> </TD><TR>' . " \n ";
			print '<TR><TD ALIGN="LEFT" VALIGN="top" CLASS="td_label">Search by Ticket Number (Num.) &nbsp;</TD>' ."\n";
			print '<TD><INPUT TYPE="text" SIZE="4" MAXLENGTH="4" VALUE="" NAME="id"></TD>' ."\n";
			print '<TD ALIGN="LEFT"><div align="left"><INPUT TYPE="submit" VALUE="Go"></div></TD></TR>' ."\n";			
			print '</TABLE></FORM>' ."\n";
			print "</TD> \n ";
		}	
		if ($search_help_is_activated)
		{
			// Guide to use Search form
			print '<TD VALIGN="top"> ';
			print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
			print '<THEAD><TR ALIGN="CENTER" ><TD COLSPAN="2" CLASS="td_link"> Search Help </TD></TR></THEAD>' ."\n";
			print '<TR><TD VALIGN="top" CLASS="td_label">Query: &nbsp;</TD>' ."\n";
			print '</TR></TABLE>' ;
			print "</TD> \n ";
		}
		// Action Search
		// FIXME : as it has to be "actionized"
		if ($action_is_activated)
		{
			print '<TD VALIGN="top"> ';
			print '<FORM METHOD="post" ACTION="search.php?term=action">' ."\n";
			print '<TABLE CELLPADDING="2" BORDER="0">' ."\n";
			print '<THEAD><TR ALIGN="CENTER" ><TD COLSPAN="2" CLASS="td_link"> Enter Actions\' Search Terms </TD></TR></THEAD>' ."\n";
			print '<TR><TD VALIGN="top" CLASS="td_label">Query: &nbsp;</TD>' ."\n";
			print '<TD><INPUT TYPE="text" SIZE="40" MAXLENGTH="255" VALUE="'.$_POST['frm_query'].'" NAME="frm_query"></TD></TR>' ."\n";
			print '<TR><TD VALIGN="top" CLASS="td_label">Search in: &nbsp;</TD>' ."\n";
			print '<TD><SELECT NAME="frm_search_in">' ."\n";
			print '<OPTION VALUE="" checked>All</OPTION>' ."\n";
			print '<OPTION VALUE="owner">Owner</OPTION>' ."\n";
			print '<OPTION VALUE="description">Description</OPTION>' ."\n";
			print '<OPTION VALUE="t_date">Issue Date</OPTION>' ."\n";
			print '</SELECT></TD></TR>' ."\n";
			
			
			print '<TR><TD VALIGN="top" CLASS="td_label">Order By: &nbsp;</TD>' ."\n";
			print '<TD><SELECT NAME="frm_ordertype">' ."\n";
			print '<OPTION VALUE="owner">Owner</OPTION>' ."\n";
			print '<OPTION VALUE="description">Description</OPTION>' ."\n";
			print '<OPTION VALUE="t_date">Issue Date</OPTION>' ."\n";
			print '</SELECT>' ."\n";
			
			print '&nbsp;Descending: <INPUT TYPE="checkbox" NAME="frm_order_desc" VALUE="DESC" CHECKED></TD></TR>' ."\n";
			print '</TD></TR>' ."\n";
			print '<TR><TD></TD>' ."\n";
			print '<TD><INPUT TYPE="submit" VALUE="Submit Search"></TD></TR>' ."\n";
			print '</TABLE></FORM>' ."\n";
			print ' </TD>' ."\n";
		}			
		print '</TR></TABLE>' ."\n";
	break;
	case ("ticket"):
		$_POST['frm_query'] = ereg_replace(' ', '|', $_POST['frm_query']);

		//what field are we searching?
		if($_POST['frm_search_in'])
		{
			$search_fields = "$_POST[frm_search_in] REGEXP '$_POST[frm_query]'";
		}
		else
		{
			// 'ticket' fields we are going to be searching . First, the fields that need "LIKE"
			$fieldnames = array("t_date", "problemstart","problemend");
			//list fields and form the query to search all of them
			if ($_POST['frm_query'])
			{			
				while ($field2search = each ($fieldnames))
				{
					$search_fields .= "`".$field2search[1]."` LIKE '%$_POST[frm_query]%' OR ";
				}
				// 'ticket' fields we are going to be searching . First, the fields that need "REGEX"
				$fieldnames = array("scope","affected","description");
				while ($field2search = each ($fieldnames))
				{
					$search_fields .= "`".$field2search[1]."` REGEXP '$_POST[frm_query]' OR ";
				}
			// Remove the ending  part in the end of string $search_fields -> ' OR '
			$search_fields = " ( ".substr($search_fields,0,strlen($search_fields) - 4) ." ) ";
			}
			else
			{
				$search_fields = "";
			}
			if ($debug)
			{
				print '<br>$search_fields ="'. $search_fields ."\" \n ";
			}
			}			
			//is user restricted to his/her own tickets?
			if (get_variable('restrict_user_tickets') && !($_SESSION['ticket_user_is_admin']))
			{
				$restrict_ticket = " AND (owner='$_SESSION[user_id]') ";
				if ($debug)
				{
					print '<br>$restrict_ticket ="'. $restrict_ticket ."\" \n ";
				}				
			}
			// Only show tickets from selected user
			if (trim($_POST['frm_owner']) )
			{
				$restrict_ticket =  " (owner='".$_POST['frm_owner']."') ";
			}
			if ($debug)
			{
				print '<br>$restrict_ticket ="'. $restrict_ticket ."\" \n ";
			}
			//tickets
			$query = "SELECT *,  DATE_FORMAT(t_date,' %m/%d/%Y') AS `t_date`, ";
			//$query = "SELECT *, ";
			$query = $query . " DATE_FORMAT(problemstart,' %m/%d/%Y %H:%i') AS `problemstart`, ";
			$query = $query . " DATE_FORMAT(problemend,' %m/%d/%Y %H:%i') AS `problemend` FROM `ticket` WHERE 1 ";
			if ($_POST['frm_querytype'] != "%")
			{
				$query = $query . " AND (`status`='$_POST[frm_querytype]') ";
			}
			if ($search_fields != "")
			{
				$query = $query . " AND $search_fields ";
			}
			if ($restrict_ticket != "")
			{				
				$query = $query . " AND $restrict_ticket ";
			}
			$query = $query . " ORDER BY `$_POST[frm_ordertype]` $_POST[frm_order_desc]";
			if ($debug) // (1) //
			{
				print '<br>Search $query ="'. $query ."\" \n ";
			}
			$result = mysql_query($query) 
				or do_error('search.php','search query <br>"'.$query.'"<br>failed, possibly illegal syntax', mysql_error());
			if ($debug)
			{
				print '<br>mysql_num_rows($result) ="'. mysql_num_rows($result) ."\" \n ";
			}	
			switch(mysql_num_rows($result))
			{
			case 0: // No tickets found
				print '<H3>No matching tickets found.</H3>';
				//do_title('No matching tickets found with query: "'.$_POST[frm_query].'"');
			break;
			case 1:
				// display ticket in whole if just one returned
				$row = mysql_fetch_array($result);
				//show_ticket($row[id]);
				show_data($row[id],$GLOBALS['MAIN_TABLE'],$GLOBALS['ELEMENT'],$page,$start_id=0);
				//add_footer($row[id]);
				powered();
				exit();
			break;
			default:
				do_title('Search results for "'.$_POST[frm_query].'"');
				$result_fields = array ("Ticket", "Date", "Severity", "Description", "Affected", "Owner", "Closed?");
				$ticket_found = 1;
				print '<TABLE BORDER="0"><TR>';
				for ($i = 0; $i < sizeof ($result_fields); $i++)
				{
					print '<TD CLASS="td_header">'.$result_field[$i].'</TD>';
				}
				print "</TR> \n ";
				while($row = mysql_fetch_array($result))
				{
					print '<TR><TD><A HREF="'.$GLOBALS['MAIN_PAGE'].'.php?id='.$row[id].'">#'.$row[id].'&nbsp;&nbsp;</A></TD>' . " \n ";
					print "<TD>".$row[t_date]."&nbsp;&nbsp;&nbsp;</TD>\n";
					print "<TD>".get_single_option("severity", $row[severity])."&nbsp;&nbsp;&nbsp;</TD>\n";
					if (get_variable('abbreviate_description'))
					{
							$wrap_var=" NOWRAP ";
							if (strlen($row[description]) > get_variable('abbreviate_description')) 
							{
								$row[description] = substr($row[description],0,get_variable('abbreviate_description')).'...';
							}
					}
					if (get_variable('abbreviate_affected'))
					{
							$wrap_var=" NOWRAP ";
							if (strlen($row[affected]) > get_variable('abbreviate_affected')) 
							{
								$row[affected] = substr($row[affected],0,get_variable('abbreviate_affected')).'...';
							}
					}
					print '<TD><A HREF="'.$GLOBALS['MAIN_PAGE'].'.php?id='.$row[id].'">'.$row[description].'</A></TD>';
					print "<TD>".$row[affected]."</TD>\n";
					print "<TD>".get_owner($row[owner])."&nbsp;&nbsp;</TD>\n";
					print "<TD>".get_single_option("status", $row[status])."&nbsp;&nbsp;&nbsp;</TD></TR\n";					
				}
				print '</TABLE><BR><BR>';
			break;
			}
			if ($action_is_activated)
			{	
				/************************************************************
				*****
				*****		All this section needs serious fixin'. It's just a copy-paste/place-holder thing.
				*****
				************************************************************/
				//actions
				//$query = "SELECT *,UNIX_TIMESTAMP(date) AS date FROM action WHERE description REGEXP '$_POST[frm_query]'";
				$query = "SELECT *,  DATE_FORMAT(t_date,' %m/%d/%Y') AS `t_date` ";
				$query = $query . " FROM `action` WHERE `description` REGEXP '$_POST[frm_query]'";
				$result = mysql_query($query) or 
					do_error('search.php','search query  <br>'.$query.'<br>failed, possibly illegal syntax', mysql_error());
				if(mysql_num_rows($result) && !$ticket_found)
				{
					// display ticket in whole if just one returned
					$row = mysql_fetch_array($result);
					show_ticket($row[ticket_id]);
					//add_footer($row[id]);
					powered();
					exit();
				}
				else if (mysql_num_rows($result) == 1)
				{
					print '<TABLE BORDER="0"><TR><TD CLASS="td_header">Ticket</TD>'."\n";
					print '<TD CLASS="td_header">Date</TD>'."\n";
					print '<TD CLASS="td_header">Action</TD></TR>'."\n";
					while($row = mysql_fetch_array($result))
					{
						print "<TR><TD VALIGN=\"top\">#$row[ticket_id]&nbsp;&nbsp;</TD>"."\n";
						print "<TD NOWRAP VALIGN=\"top\">".$row[t_date]."&nbsp;&nbsp;&nbsp;</FONT></TD>"."\n";
						print '<TD><A HREF="'.$GLOBALS['MAIN_PAGE'].'.php?id='.$row[ticket_id].'">'.$row[description].'</A></TD></TR>\n';
					}
					print '</TABLE>';
				}
				else
				{
					print 'No matching actions found.';
				}
			}		
		//}
		/***********************************************************
		else
		{
			//print '<H2 CLASS="header">Search</H2>';
			do_title('Search for '.$GLOBALS['ELEMENT'].'s');
			// Show list of existing unique elements?
			//***************************************
			//	Well, not just yet
			//***************************************
			if ($action_is_activated)
			{
				$these_fields=array('affected', 'scope');
				while ($this_field = each ($these_fields))
				{
					$search_query = "SELECT DISTINCT `$this_field` FROM `ticket` LIMIT 1";
					if ($debug)
					{
						print '<br>$this_field ="'.$this_field .'\"';
						print '<br>$search_query ="'.$search_query .'\"';
					}
					$search_result = mysql_query($search_query)  
						or do_error('search.php','search query, possibly illegal syntax with $search_query="'.$search_query.'"', mysql_error());
					print "<TABLE BORDER=\"0\">";
					print "<TR><TD CLASS=\"td_header\">Ticket</TD>\n";
					print "<TD CLASS=\"td_header\">Date</TD>\n";
					print "<TD CLASS=\"td_header\">Description</TD>\n";
					print "<TD CLASS=\"td_header\">Affected</TD>\n";
					print "<TD CLASS=\"td_header\">Owner</TD>\n";
					print "<TD CLASS=\"td_header\">Closed?</TD></TR>\n";
					while($search_row = mysql_fetch_array($search_result))
					{
						print "<TR><TD><A HREF=\"search.php?\">#$row[id]&nbsp;&nbsp;</A></TD>\n";
						print "<TD>".$search_row[t_date]."&nbsp;&nbsp;&nbsp;</TD>\n";
						print "<TD><A HREF=\"main.php?id=$row[id]\">$row[description]</A></TD>\n";
						print "<TD>".$search_row[affected]."</TD>\n";
						print "<TD>".get_owner($search_row[owner])."&nbsp;&nbsp;</TD></TR>\n";
						print "<TD>".get_single_option("status", $search_row[status])."&nbsp;&nbsp;&nbsp;</TD></TR>\n";
					}
					print '</TABLE><BR><BR>';
				}	
			}
		} 
		***********************************************************/
	break;
	case ("action"):
	break;
}


powered(); // Print "Powered by" and end the HTML page
die;
?>
