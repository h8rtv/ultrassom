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

function clear_images() {
    var rows = document.querySelectorAll("#table tr.image-data");
    for (var i = 0; i < rows.length; i++) {
        rows[i].parentNode.removeChild(rows[i]);
    }
}

function create_images(data) {
    var json = JSON.parse(data);

    var table = document.querySelector("#table");
    var tbody = document.createElement("tbody");
    table.appendChild(tbody);

    for (var i = 0; i < json.length; i++) {
        var image = json[i];
        var tr = document.createElement("tr");
        tr.id = "image-" + image["id"];
        tr.className = "image-data";
        tbody.appendChild(tr);

        var td_id = document.createElement("td");
        td_id.innerHTML = image["id"];
        tr.appendChild(td_id);

        var td_algo = document.createElement("td");
        td_algo.innerHTML = image["algo"];
        tr.appendChild(td_algo);

        var td_start_date = document.createElement("td");
        td_start_date.className = "start-date";
        if (image["start_date"] != null)
            td_start_date.innerHTML = formatDate(image["start_date"]);
        tr.appendChild(td_start_date);

        var td_end_date = document.createElement("td");
        td_end_date.className = "end-date";
        if (image["end_date"] != null)
            td_end_date.innerHTML = formatDate(image["end_date"]);
        tr.appendChild(td_end_date);

        var td_iterations = document.createElement("td");
        td_iterations.className = "iterations";
        if (image["iterations"] != null)
            td_iterations.innerHTML = image["iterations"];
        tr.appendChild(td_iterations);

        var td_quality = document.createElement("td");
        td_quality.className = "quality";
        if (image["quality"] != null)
            td_quality.innerHTML = getQualityString(image["quality"]);
        tr.appendChild(td_quality);

        var td_reconstruction_time = document.createElement("td");
        td_reconstruction_time.className = "time";
        if (image["time"] != null)
            td_reconstruction_time.innerHTML = formatSeconds(image["time"]);
        tr.appendChild(td_reconstruction_time);

        var td_img = document.createElement("td");
        td_img.className = "image";

        if (image["image_url"] != null) {
            var img = document.createElement("img");
            img.src = image["image_url"];
            td_img.appendChild(img);
        } else {
            td_img.innerHTML = "Processando...";
        }

        tr.appendChild(td_img);
    }
}

function reload_image(image_data) {
    var image_data = JSON.parse(image_data);

    var trRoot = document.querySelector("#image-" + image_data["id"]);
    var td_img = trRoot.querySelector("td.image");
    td_img.innerHTML = "";
    var img = document.createElement("img");
    img.src = image_data["image_url"];
    td_img.appendChild(img);

    var td_start_date = trRoot.querySelector("td.start-date");
    if (image_data["start_date"] != null)
        td_start_date.innerHTML = formatDate(image_data["start_date"]);

    var td_end_date = trRoot.querySelector("td.end-date");
    if (image_data["end_date"] != null)
        td_end_date.innerHTML = formatDate(image_data["end_date"]);

    var td_iterations = trRoot.querySelector("td.iterations");
    if (image_data["iterations"] != null)
        td_iterations.innerHTML = image_data["iterations"];

    var td_quality = trRoot.querySelector("td.quality");
    if (image_data["quality"] != null)
        td_quality.innerHTML = getQualityString(image_data["quality"]);

    var td_reconstruction_time = trRoot.querySelector("td.time");
    if (image_data["time"] != null)
        td_reconstruction_time.innerHTML = formatSeconds(image_data["time"]);
}

function getQualityString(quality) {
    if (quality == 0) {
        return "Baixa";
    } else if (quality == 1) {
        return "Média";
    } else if (quality == 2) {
        return "Alta";
    } else {
        return "Desconhecido";
    }
}

function formatSeconds(seconds) {
    // remove everything after the decimal point
    var s = seconds.toString();
    var i = s.indexOf(".");
    if (i != -1)
        s = s.substring(0, i);

    s += " s";

    return s;
}

function formatDate(date) {
    return new Date(date).toLocaleString();
}

function open_file_dialog() {
    pywebview.api.open_file_dialog();
}

function process_file() {
    document.querySelector("#main #send").disabled = true;
    pywebview.api.process_file();
}

function can_send(){
    document.querySelector("#form #send").disabled = false; // enable the send button
}

function process_done(){
    document.querySelector("#form #select").disabled = true; // disable the file select
    document.querySelector("#form #send").disabled = false; // enable the send button

    document.querySelector("#form #send").value = "Enviar imagem";
    document.querySelector("#form #send").onclick = send_file;
}

function send_file() {
    let quality = document.querySelector("#form #quality").value;
    let algo = document.querySelector("#form #algo").value;

    pywebview.api.send_image(quality, algo);
    document.querySelector("#form #send").value = "Processar";
    document.querySelector("#form #send").onclick = process_file;
    document.querySelector("#form #select").disabled = false;
}

function refresh_images() {
    pywebview.api.refresh_images();
    document.querySelector("#main .refresh").disabled = true;
}

function refresh_images_done() {
    document.querySelector("#main .refresh").disabled = false;
}