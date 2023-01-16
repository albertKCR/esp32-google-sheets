var SS = SpreadsheetApp.openById('1ELHCl-1dL5LFJFQ-sP02MA2dvbyNQNiFwArYep-9QsU');
var str = "";

function doGet(e) {
  return valOrg();
}

function valOrg() {

  var sheet = SS.getSheetByName("tempdata");
  var dataArr = new Array();
  dataArr[0] = sheet.getSheetValues(2, 4, 1, 1)[0][0];
  dataArr[1] = sheet.getSheetValues(2, 5, 1, 1)[0][0];
  dataArr[2] = sheet.getSheetValues(7, 6, 1, 1)[0][0];
  dataArr[3] = sheet.getSheetValues(7, 7, 1, 1)[0][0];
  dataArr[4] = sheet.getSheetValues(2, 6, 1, 1)[0][0];
  dataArr[5] = sheet.getSheetValues(7, 9, 1, 1)[0][0];
  dataArr[6] = sheet.getSheetValues(2, 10, 1, 1)[0][0];
  dataArr[7] = sheet.getSheetValues(2, 11, 1, 1)[0][0];

  var sheet1 = SS.getSheetByName("measureValues");

  sheet1.appendRow(dataArr);
}