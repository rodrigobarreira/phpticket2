<?PHP
/**************************************
*** File: top.php *********************
*** Project: phpticket ****************
***************************************
*** Authors: Daniel Netz <netz@home.se> and ***  
***  Sinner from the Prairy <sinnerbofh@gmai.com> ***
*** email: sinnerbofh@gmail.com *****
*** Comment: From phptcket to HIPAA Tracker back to phpticket*
**************************************/

require_once('conf.inc');
require_once('functions01.inc');
mysql_open(); 

$this_is =get_variable('version');
if ( strstr( $this_is,"devel" ) )
{
	$back_color="#BBBBBB";//$back_color="#7CB6DF";
} else {
	$back_color="#E0E0BC";//$back_color="#84b8e0";	
}

print '<HTML><HEAD>
		<TITLE>'.$this_title .' -  .' . get_variable('version') . '</TITLE>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<style type="text/css">';
print '<!--';

print 'BODY { BACKGROUND-COLOR: '.$back_color.'; FONT-WEIGHT: normal; FONT-SIZE: 11px; 
	COLOR:#000000; FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; TEXT-DECORATION:none }
h4 { FONT-SIZE: 10px;  FONT-WEIGHT: bold;  FONT-STYLE: normal;  FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; 
	TEXT-DECORATION: none; margin-top: 0em; margin-bottom: 0em;}	
#this 	{ FONT-SIZE: 10px;  FONT-WEIGHT: bold;  FONT-STYLE: normal;  FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; 
	TEXT-DECORATION: none; margin-top: 0em; margin-bottom: 0em;}
a { FONT-WEIGHT: bold; FONT-SIZE: 11px; COLOR: #000099; FONT-STYLE: normal;
FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif; }
a:link {  FONT-WEIGHT: bold;  COLOR: #000099;  FONT-STYLE: normal;
	padding-left: 0.5em; padding-right: 0.5em; 
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;  TEXT-DECORATION: none; }
a:visited {  FONT-WEIGHT: bold;  COLOR: #000099;  FONT-STYLE: normal; 
	padding-left: 0.5em; padding-right: 0.5em;
	FONT-FAMILY: Verdana, Arial, Helvetica, sans-serif;  TEXT-DECORATION: none; 	}
a:hover, a:active {  text-decoration: underline;  BACKGROUND-COLOR: #ddd; 
	padding-left: 0.5em; padding-right: 0.5em;  }	
#navhor{ margin-top: 1em; }
	
#navhor ul { BACKGROUND-COLOR: #EEEEEE;  text-align: center; margin-left: 0; padding-left: 0;
	border-top: 1px solid silver; border-bottom: 1px solid gray; }
	
#navhor li { list-style-type: none; padding: 0.25em 1em; border-left: 1px solid white; display: inline }
	
#navhor li:first-child { border: none; }
-->
</style>
</HEAD>' ."\n";

print '<!--BACKGROUND-COLOR: #fff5e3;#EEEEEE; -->' ." \n ";
print '<BODY>';

print '<TABLE width="90%" cellpadding=0 cellspacing=0 border=0><tr><td align ="left">';
print '<h4>'.$this_title . '  V. '. get_variable('version'). ' at <a href="http://'.get_variable('host').
		'" target="_blank">'.get_variable('host').'</a> </h4>';  
print '</TD><TD align="right">';
print '<A HREF="help.php" target="main">Help </A>'."&nbsp;&nbsp;&nbsp; " . '</TD><TD>';
print ' <A HREF="logout.php" target="main"> Logout</A>  ';
print '</TD></TR><TR><td COLSPAN="3">';

if ($GLOBALS['SEVERITY_SORT'])
{
	$button_link = array ( "main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0", 
		"main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=1" , 
		"main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=2", 
		"main.php?action=add&id=0",
		"main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0&showclosed=true",
		"main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=1&showclosed=true" ,
		"main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=2&showclosed=true"
		);
	$button_label = array ( "[All, " ,"Medium+, ", "High] ", "Add New Ticket ", "[All, ", "Medium+, ", "High]");

	print '<div id="this">Open Tickets ';
	for ($i = 0; $i < 4; $i++)
	{
		print '<A HREF="'.$button_link[$i].'" target="main">'.$button_label[$i].'</A>';
		//$whattoprint = ($i = 2) ? (", ") : ("] | ") ;
		//print $whattoprint;
		
	}
	/*******
	print '<A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0" target="main">All</A> , ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=1" target="main">Medium+</A> , ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=2" target="main">High</A>] | ';
	print '<A HREF="main.php?action=add&id=0" target="main">Add New Ticket</A> | ';
	*****/
	print 'Closed Tickets ';
	for ($i = 4; $i < sizeof($button_link); $i++)
	{
		print '<A HREF="'.$button_link[$i].'" target="main">'.$button_label[$i].'</A>';
		//print ($i < 2) ? (" , ") : (" | ");
	}
	/*****************************
	print '<A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0&showclosed=true" target="main">All</A> , ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=1&showclosed=true" target="main">Medium+</A> , ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=2&showclosed=true" target="main">High</A>] | ';
	*****************************/
} else {
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0" target="main">All</A> | ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0&unassigned='.$GLOBALS['STATUS_ASSIGNED'].'" target="main">Open Tickets</A> | ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0&unassigned='.$GLOBALS['STATUS_UNASSIGNED'].'" target="main">Un-assigned Tickets</A> | ';
	print '<A HREF="main.php?action=add&id=0" target="main">Add New Ticket</A> | ';
	print ' <A HREF="main.php?ident=id&current=0&order=DESC&sort_by=t_date&severity=0&showclosed=true" target="main">Closed Tickets</A> | ';
}

print ' <A HREF="search.php" target="main">Search </A>  ';
print ' <A HREF="config.php" target="main">Settings</A> ';

print '</div></TD></TR></TABLE>';


print '</body></html> ';
?>
