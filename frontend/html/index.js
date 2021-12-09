function clear_images() {
    var rows = document.querySelectorAll("#table tr.image-data");
    for (var i = 0; i < rows.length; i++) {
        rows[i].parentNode.removeChild(rows[i]);
    }
}

function set_image_inner_html(image, td_img) {
    td_img.innerHTML = "";
    switch (image["status"]) {
        case 0:
            td_img.innerHTML = "Na fila...";
            break;
        case 1:
            td_img.innerHTML = "Processando...";
            break;
        case 2:
            if (image["image_url"] != null) {
                var img = document.createElement("img");
                img.src = image["image_url"];
                img.onclick = open_modal.bind(null, image["image_url"]);
                td_img.appendChild(img);
            }
            break;
        default:
            td_img.innerHTML = "Falha ao processar";
            break;
    }
}

function set_download_inner_html(image, td_download) {
    td_download.innerHTML = "";
    switch (image["status"]) {
        case 2:
            if (image["image_url"] != null) {
                var link = document.createElement("button");
                link.classList.add("download-link");
                link.innerHTML = "Download";
                link.onclick = function() {
                    pywebview.api.save_image(image["image_url"]);
                }
                td_download.appendChild(link);
            }
            break;
        default:
            td_download.innerHTML = "-";
            break;
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
        td_quality.className = "qualityc";
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
        set_image_inner_html(image, td_img);
        tr.appendChild(td_img);

        var td_download = document.createElement("td");
        td_download.className = "download";
        set_download_inner_html(image, td_download);
        tr.appendChild(td_download);
    }
}

function reload_image(image_data) {
    var image_data = JSON.parse(image_data);

    var trRoot = document.querySelector("#image-" + image_data["id"]);

    var td_img = trRoot.querySelector("td.image");
    set_image_inner_html(image_data, td_img);

    var td_download = trRoot.querySelector("td.download");
    set_download_inner_html(image_data, td_download);

    var td_start_date = trRoot.querySelector("td.start-date");
    if (image_data["start_date"] != null)
        td_start_date.innerHTML = formatDate(image_data["start_date"]);

    var td_end_date = trRoot.querySelector("td.end-date");
    if (image_data["end_date"] != null)
        td_end_date.innerHTML = formatDate(image_data["end_date"]);

    var td_iterations = trRoot.querySelector("td.iterations");
    if (image_data["iterations"] != null)
        td_iterations.innerHTML = image_data["iterations"];

    var td_quality = trRoot.querySelector("td.qualityc");
    if (image_data["quality"] != null)
        td_quality.innerHTML = getQualityString(image_data["quality"]);

    var td_reconstruction_time = trRoot.querySelector("td.time");
    if (image_data["time"] != null)
        td_reconstruction_time.innerHTML = formatSeconds(image_data["time"]);
}

function open_modal(image_url) {
    document.querySelector("#image-modal").style.display = "block";
    document.querySelector("#image-modal-content").src = image_url;
}

function close_modal() {
    document.querySelector("#image-modal").style.display = "none";
    document.querySelector("#image-modal-content").src = "";
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
    var i = s.indexOf("."); // find the decimal point
    var l = (i + 4 > s.length) ? s.length : i + 4; // 3 decimal places
    if (i != -1)
        s = s.substring(0, l);

    s += " s";

    return s;
}

function formatDate(date) {
    return new Date(date).toLocaleString();
}
