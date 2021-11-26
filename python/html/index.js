function onLoad() {

}

// function submitFile() {
//     var file = document.getElementById("file").files[0];
//     if (!file) {
//         alert("Please choose a file to upload!");
//         return;
//     }

//     var reader = new FileReader();
//     reader.readAsText(file, "UTF-8");

//     reader.onload = function (evt) {
//         pywebview.api.process_file(evt.target.result);
//         document.querySelector("#form #send").disabled = true;
//         document.querySelector("#form #send").value = "Processing...";
//     }
//     reader.onerror = function (evt) {
//         alert("Error reading file");
//     }

// }

function process_done(){
    pywebview.api.echo("done()");
    document.querySelector("#form #send").disabled = false;
    document.querySelector("#form #send").value = "Send image";
    document.querySelector("#form #send").onclick = function(){
        pywebview.api.send_image();
    }
}

function file_selected(isSelected) {
    // document.querySelector("#form #select").disabled = isSelected;
}