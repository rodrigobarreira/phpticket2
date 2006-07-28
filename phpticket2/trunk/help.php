<?PHP
/**************************************   
*** File: help.php     ****************
*** Project: phpticket ****************
***************************************
*** Authors: Daniel Netz <netz@home.se> and ***  
***  Sinner from the Prairy <sinnerbofh@gmai.com> ***
*** email: sinnerbofh@gmail.com *****
*** Comment: From phptcket to HIPAA Tracker back to phpticket*
**************************************/

	require_once('conf.inc');
	require_once('functions01.inc');

print '<HTML>
<HEAD>
<LINK REL=StyleSheet HREF="default.css" TYPE="text/css">
</HEAD><BODY>';
do_title("Help");

print '
<LI> <A HREF="help.php?q=Tickets">Tickets and Actions</A>
<LI> <A HREF="help.php?q=configuration">Configuration</A>
<!-- <LI> <A HREF="help.php?q=notify">Notifies</A>-->
<LI> <A HREF="help.php?q=settings">Settings</A></h3>
<LI> <A HREF="help.php?q=contact">Getting Help</A></h3>
<!-- <LI> <A HREF="help.php?q=install">Install</A>-->
<!-- <LI> <A HREF="help.php?q=readme">ReadMe</A>>-->
<LI> <A HREF="help.php?q=develop">Development</A>
<!-- <LI> <A HREF="help.php?q=changelog">ChangeLog</A>-->
<!-- <LI> <A HREF="help.php?q=todo">ToDo</A>-->
<LI> <A HREF="help.php?q=licensing">Licensing</A>
<LI> <A HREF="help.php?q=credits">Credits</A>
<BR><BR>' ."\n";


switch ($_GET['q'] )
{
case 'Tickets':
	print '<h3>'.ucfirst($_GET['q']) .' and Actions</h3>';
	print '<P><b>Tickets</b> are what describes one or several issues, problems if you wish. Every ticket may have one or ';
	print 'several <b>actions</b> related to it to describe work in progress or adding side notes (actions are described below.) ';
	print '<p>A ticket contains several values to describe the issue, starting with <B>ID</B> which is of no real use to ';
	print 'the user. <B>Issue date</B> is at what date and time the ticket was created, <B>problem start</B> and ';
	print '<B>problem end</B> date and time for when the issue starts and ends. The <B>Contact Info</B> may be used to  ';
	print 'keep track of who called for help. In this 0.9xx version, "Contact Info" substitutes the "Type of Problem" and "Scope"  ';
	print ' of previous versions. The <B>owner</B> field corresponds to the user the ticket belongs to, not ';
	print 'neccesarily the creator. It starts set with the special user <b>Un-assigned</b>';
	print '<P>The <B>affected</B> field explains what is affected by this issue, like departmens, computer name, ';
	print 'groups of people or anything you want. The <B>status</B> field is either open or closed depending ';
	print 'on the ticket status. Closed tickets can be made open again by changing the status again. Removed tickets ';
	print 'however are deleted completely from the database along with its related actions. The <B>description</B> ';
	print 'field describes the issue in depth.';
	print '<P>When the issue described in a ticket is updated, <B>actions</B> may be added to reflect that change. ';
	print 'Actions are simply a string value with a date for when the action was added. ';
break;
case 'configuration':
	print '<h3>'.ucfirst($_GET['q'] ).'</h3>';
	print "<p>Ticket2 configuration requires several steps.<p>";
	print "<ol>
	<li>Edit file <b><tt>conf.inc</tt></b>  - with proper MySQL connection data (MysQL username & password, table name, 
		MySQL server name, ticket2 database name, emails for each new ticket and sort type)</li>
	<li>Edit of contents of table <b><tt>settings</tt></b> - insert proper values for Login Banner; administrator's name, 
		email and phone number; and company's name , department and website.</li>
	<li>Add <b>locations</b> - Edit the built-in sample locations (<tt>Location 1, Location 2, Location 3</tt>) and add
		any new desired locations.</li>
	
		</ol>";
break;
case 'notify':
	print '<h3>'.ucfirst($_GET['q'] ).'</h3>';
break;
case 'setings':
		print '<H3>'.ucfirst($_GET['q'] ).'</H3>';
		print "<p>The Settings section of this program provides user management, administrative settings and database maintenance. 
		<p>A <b>regular user</b> can change his/her password in this section. 
		<p> An <b>administrator user</b> has extended options. With the extended options, an <b>administrator</b> 
		can create, edit and delete  users.  Users can have the <b>administrator</b> flag. This user flag toggles 
		<b>administrator</b> rights. 'Settings' is used to control various variables in this program and should  be carefully 
		changed since there is no check for correct values. If you are an <b>administrator</b>, you should be careful.";
break;
case 'contact':
		print '<FONT CLASS="header"><BR>Getting Help</FONT><BR><BR>';
		print '<p>If you have any problem, suggestion or comment about this program, please contact:
		<p><table><tr><td class="td_label" >'.get_variable('admin').'</td><td>IT Department, '.get_variable('organization').'</td></tr>
		<tr><td class="td_label">Phone:</td><td>'.get_variable('admin_phone').'</td></tr>
		<tr><td class="td_label">email: </td><td>'.get_variable('admin_email').'</td></tr></table>';
break;
case 'install':
	print '<h3>'.ucfirst($_GET['q']) .'</h3>';
break;
case 'readme':
	print '<h3>'.ucfirst($_GET['q'] ).'</h3>';
break;
case 'develop':
	print '<h3>'.ucfirst($_GET['q']) .'</h3>';
break;
case 'changelog':
	print '<h3>'.ucfirst($_GET['q']) .'</h3>';
break;
case 'todo':
	print '<h3>'.ucfirst($_GET['q']) .'</h3>';
break;
case 'licensing':
	print '<h3>'.ucfirst($_GET['q']) .'</h3>';
break;
case 'credits':
	print '<h3>'.ucfirst($_GET['q']) .'</h3>';
	print '<ul>
		<li>Programming by Sinner from the Prairy, sinnerbofh "at" gmail "dot" com </i>
		<li>Loosely based on <b>PHP Ticket</b>, by Daniel Netz, netz "at" home "dot" se ...</A> and Sinner from the Prairy, sinnerbofh "at" gmail "dot" com</i>
		<li>... and on <b>Athena Access Portal</b>, by Sinner from the Prairy, sinnerbofh "at" gmail "dot" com 
			(<A HREF="http://www.ibiblio.org/sinner/" target="new">Web Site</A>)</i>
		<li>PHP Ticket SourceForge Project: <A HREF="http://www.sourceforge.net/projects/ticket/" 
			target="new">sourceforge.net/projects/ticket/</A></i>
		<li>Ticket2, Athena Access Portal &amp; PHP Ticket are licensed under 
		<A HREF="http://www.gnu.org/licenses/gpl.html" target="new">GPL</A>.</i>
		<li>Thanks to <A HREF="http://www.apache.org" TARGET="new">Apache</A>, 
		<A HREF="http://www.php.net" TARGET="new">PHP</A>, 
		<A HREF="http://www.mysql.com" TARGET="new">MySQL</A>, 
		<A HREF="http://www.jedit.org" TARGET="new">JEdit</A> and OpenSource in all.</li>
		<li>Special thanks to everyone contributing with ideas, code snippets and reporting problems.</li>
		</ul>';	
break;
}
if ($_GET['q'] == 'contact')
	{
		print '<FONT CLASS="header"><BR>Getting Help</FONT><BR><BR>';
		print '<p>If you have any problem, suggestion or comment about this program, please contact:
		<p><table><tr><td class="td_label" >'.get_variable('admin').'</td><td>IT Department, '.get_variable('organization').'</td></tr>
		<tr><td class="td_label">Phone:</td><td>'.get_variable('admin_phone').'</td></tr>
		<tr><td class="td_label">email: </td><td>'.get_variable('admin_email').'</td></tr></table>';
		
	}
	else if ($_GET['q'] == 'setting')
	{
		print '<FONT CLASS="header"><BR>Settings</FONT><BR>';
		print "<p>The Settings section of this program provides user management, administrative settings and database maintenance. 
		<p>A <b>regular user</b> can change his/her password in this section. 
		<p> An <b>administrator user</b> has extended options. With the extended options, an <b>administrator</b> 
		can create, edit and delete  users.  Users can have the <b>administrator</b> flag. This user flag toggles 
		<b>administrator</b> rights. 'Settings' is used to control various variables in this program and should  be carefully 
		changed since there is no check for correct values. If you are an <b>administrator</b>, you should be careful.";
	}
	else if ($_GET['q'] == 'develop')
	{
		print '<FONT CLASS="header"><BR>Development</FONT><BR>';
		print "<p>Developing this program to suit your particular needs shouldn't be very hard.  The actual PHP 
		code is fairly simple and easy to edit, while the HTML code that make up the interface is less simple 
		to  change. The font properties, table backgrounds etc, works through the use of CSS (default.css) for easy editing.
		<p>Most of the most basic functions are  located in the functions.inc.php file. Most advanced functions 
		are located in the functions02.inc file. To add a setting, just add the line in the  \"settings\" table 
		in the database and it'll show up on the settings screen.";
				
		print '<p>All data is stored in a <b>MySQL</b> database. The tables structure is the following:
		<ul>
		<li><b>tickets</b> contains ticket data.  </li>
		<li><b>options</b> contains the options shown on the different drop-down boxes. </li>
		<li><b>settings</b> table contains various settings variables, cosmetics and functional. </li>
		<li><b>actions</b> contains tickets\' actions data.  </li>
		<li><b>user</b> is used for simple authentication of users, </li>
		</ul>';
		
	}
/*	else if ($_GET['q'] == 'install') {
		print '<PRE>'; readfile('INSTALL'); print '</PRE>';
	}*/
	else if ($_GET['q'] == 'readme') {
		print '<PRE>'; readfile('README'); print '</PRE>';
	}/*
	else if ($_GET['q'] == 'todo') {
		print '<PRE>'; readfile('TODO'); print '</PRE>';
	}*/
	else if ($_GET['q'] == 'licensing') {
		print '<PRE>'; readfile('COPYING'); print '</PRE>';
	}
	powered(); // Print "Powered by" and end the HTML page
	print '</BODY></HTML>';
?>

