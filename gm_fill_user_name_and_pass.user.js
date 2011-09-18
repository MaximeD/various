// ==UserScript==
// @name           script name
// @namespace      url
// @include        url
// ==/UserScript==

// This greasemonkey script that will autofill your username and password
// on desired webpages.
// The easiest way to set it up
// is to click on 'add a new gm script'
// and to paste the following content.
// Please use a tool like a dom inspector
// to get the value of the different fields.
// Please don't consider using it on websites where security matters !

var name = "enter username here"
var pass = "enter password"

var zTextFields = document.getElementsByTagName("input");
for (var i=0; i < zTextFields.length; i++) {
    thefield=zTextFields[i].name;
    if (!thefield) thefield=zTextFields[i].id;
    
    // Set up your auto-fill values here
    if (thefield == "put field name here") zTextFields[i].value= name ;
    if (thefield == "put field name here") zTextFields[i].value= pass ;
}
