function handleLogin() {
    var username = document.querySelector("#username").value;
    pywebview.api.login(username).then(function(data) {
        if (data == true) {
            document.querySelector("#login").style.display = "none";
            document.querySelector("#main").style.display = "block";
        }
    });
}

function process_done(){
    document.querySelector("#form #send").disabled = false;
    document.querySelector("#form #send").value = "Enviar imagem";
    document.querySelector("#form #send").onclick = function(){
        let quality = document.querySelector("#form #quality").value;
        let algo = document.querySelector("#form #algo").value;

        pywebview.api.send_image(quality, algo);
        document.querySelector("#form #send").value = "Processar";
        document.querySelector("#form #send").onclick = process_file;
    }
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
        tr.className = "image-data";
        tbody.appendChild(tr);

        var td_name = document.createElement("td");
        td_name.innerHTML = image["username"];
        tr.appendChild(td_name);

        var td_algo = document.createElement("td");
        td_algo.innerHTML = image["algo"];
        tr.appendChild(td_algo);

        var td_start_date = document.createElement("td");
        if (image["start_date"] != null)
            td_start_date.innerHTML = formatDate(image["start_date"]);
        tr.appendChild(td_start_date);

        var td_end_date = document.createElement("td");
        if (image["end_date"] != null)
            td_end_date.innerHTML = formatDate(image["end_date"]);
        tr.appendChild(td_end_date);

        var td_iterations = document.createElement("td");
        if (image["iterations"] != null)
            td_iterations.innerHTML = image["iterations"];
        tr.appendChild(td_iterations);

        var td_quality = document.createElement("td");
        if (image["quality"] != null)
            td_quality.innerHTML = getQualityString(image["quality"]);
        tr.appendChild(td_quality);

        var td_reconstruction_time = document.createElement("td");
        if (image["time"] != null)
            td_reconstruction_time.innerHTML = formatSeconds(image["time"]);
        tr.appendChild(td_reconstruction_time);

        var td_img = document.createElement("td");

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
    pywebview.api.process_file();
}