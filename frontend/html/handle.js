function handleLogin() {
    var username = document.querySelector("#username").value;
    pywebview.api.login(username).then(function(data) {
        if (data == true) {
            document.querySelector("#login").style.display = "none";
            document.querySelector("#main").style.display = "block";
            document.querySelector("#user").style.display = "block";
            document.querySelector("#user").innerHTML = username;
        }
    });
}

// ========================== OPEN FILE SELECT DIALOG
function open_file_dialog() {
    pywebview.api.open_file_dialog();
}
function on_file_selected(filename) {
    document.querySelector("#form #process").disabled = false;
    document.querySelector("#form #submit").disabled = false;

    document.querySelector("#form #filename").style.display = "inline-block";
    document.querySelector("#form #filename").innerHTML = filename;
}
function file_read_error() {
    document.querySelector("#form #process").disabled = true;
    document.querySelector("#form #submit").disabled = true;
    window.alert("Erro ao ler arquivo");
}
// ========================== OPEN FILE SELECT DIALOG


// ========================== PROCESS FILE
function process_file() {
    document.querySelector("#form #select").disabled = true;
    document.querySelector("#form #process").disabled = true;
    document.querySelector("#form #submit").disabled = true;
    pywebview.api.process_file();
}
function process_done(){
    document.querySelector("#form #process").disabled = true;
    document.querySelector("#form #select").disabled = false;
    document.querySelector("#form #submit").disabled = false;
}
function process_error(){
    document.querySelector("#form #process").disabled = false;
    document.querySelector("#form #select").disabled = false;
    document.querySelector("#form #submit").disabled = false;
    window.alert("Erro ao processar arquivo");
}
// ========================== PROCESS FILE



// ========================== SUBMIT FILE
function can_send(){
    document.querySelector("#form #send").disabled = false; // enable the send button
}
function send_file() {
    let quality = document.querySelector("#form #quality").value;
    let algo = document.querySelector("#form #algo").value;

    pywebview.api.send_image(quality, algo);
}
// ========================== SUBMIT FILE



// ========================== REFRESH IMAGES
function refresh_images() {
    pywebview.api.refresh_images();
    document.querySelector("#main #refresh").disabled = true;
}

function refresh_images_done() {
    document.querySelector("#main #refresh").disabled = false;
}
// ========================== REFRESH IMAGES