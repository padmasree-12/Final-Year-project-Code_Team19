function doGet(e) { 
Logger.log( JSON.stringify(e) );
var result = 'Ok';
if (e.parameter == 'undefined') {
result = 'No Parameters';
}
else {
var sheet_id = '1byb5oSjol35w51ZPA7NNA-0oYnKfLmIifMINLbEwEX8'; // Spreadsheet ID
var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
var newRow = sheet.getLastRow() + 1; 
var rowData = [];
var Curr_Date = new Date();
rowData[0] = Curr_Date; // Date in column A
var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Kolkata", 'HH:mm:ss');
rowData[1] = Curr_Time; // Time in column B
for (var param in e.parameter) {
Logger.log('In for loop, param=' + param);
var value = stripQuotes(e.parameter[param]);
Logger.log(param + ':' + e.parameter[param]);
switch (param) {
case 'Alcohol':
rowData[2] = value; // Temperature in column C
result = 'Alcohol Written on column C'; 
break;
case 'Helmet':
rowData[3] = value; // Humidity in column D
result += ' ,Helmet Written on column D'; 
break; 
case 'Speed':
rowData[4] = value; // Humidity in column D
result += ' ,Speed Written on column D'; 
break;
case 'Location':
rowData[5] = value; // Humidity in column D
result += ' ,Speed Written on column D'; 
break;
default:
result = "unsupported parameter";
}
}
Logger.log(JSON.stringify(rowData));
var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
newRange.setValues([rowData]);
}
return ContentService.createTextOutput(result);
}
function stripQuotes( value ) {
return value.replace(/^["']|['"]$/g, "");
}