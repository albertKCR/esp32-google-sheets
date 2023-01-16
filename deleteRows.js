var SS = SpreadsheetApp.openById('1ELHCl-1dL5LFJFQ-sP02MA2dvbyNQNiFwArYep-9QsU');
var str = "";

function doGet(e) {
  return delRows();
}

function delRows() {

  var sheet1 = SS.getSheetByName("Sheet1");
  sheet1.deleteRows(2, sheet1.getLastRow());
}