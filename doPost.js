var SS = SpreadsheetApp.openById('1ELHCl-1dL5LFJFQ-sP02MA2dvbyNQNiFwArYep-9QsU');
var str = "";
 
function doPost(e) 
{
 
  var parsedData;
   
  try
  {
    parsedData = JSON.parse(e.postData.contents);
  } 
  catch(f)
  {
    return ContentService.createTextOutput("Error in parsing request body: " + f.message);
  }
    
  if (parsedData !== undefined)
  {
    var flag = parsedData.format;
    if (flag === undefined){
      flag = 0;
    }

    var sheet = SS.getSheetByName(parsedData.sheet_name);
    var dataArr = parsedData.values.split(",");

    var date_now = Utilities.formatDate(new Date(), 'America/Sao_Paulo', "dd/MM/yyyy");
    var time_now = Utilities.formatDate(new Date(), 'America/Sao_Paulo', "hh:mm:ss a");

    switch (parsedData.command) {
       
      case "insert_row":
          
        sheet.insertRows(2);
          
        sheet.getRange('A2').setValue(date_now);
        sheet.getRange('B2').setValue(time_now);
        sheet.getRange('C2').setValue(voltage);  
        sheet.getRange('D2').setValue(current);  
          
        str = "Success";
        SpreadsheetApp.flush();
        break;
          
      case "append_row":
    
        for (let i = 0; i < dataArr.length; i++){
          var publish_array = new Array();
          publish_array [0] = date_now;
          publish_array [1] = time_now;
          publish_array [2] = dataArr [i];
          publish_array [3] = dataArr [i+1];
          sheet.appendRow(publish_array);
          i=i+1;
        }
        str = "Success";
        SpreadsheetApp.flush();
        break;     
    }
     
    return ContentService.createTextOutput(str);
  }
   
  else
  {
    return ContentService.createTextOutput("Error! Request body empty or in incorrect format.");
  }
}