function onLoad() {

}

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

function create_images(data) {
    var json = JSON.parse(data);
    var images = json["images"];

    var table = document.querySelector("#table");
    var tbody = document.createElement("tbody");
    table.appendChild(tbody);

    for (var i = 0; i < images.length; i++) {
        var image = images[i];
        var tr = document.createElement("tr");
        tbody.appendChild(tr);

        var td_name = document.createElement("td");
        td_name.innerHTML = image["name"];
        tr.appendChild(td_name);

        var td_algo = document.createElement("td");
        td_algo.innerHTML = image["algo"];
        tr.appendChild(td_algo);

        var td_start_date = document.createElement("td");
        td_start_date.innerHTML = formatDate(image["start_date"]);
        tr.appendChild(td_start_date);

        var td_end_date = document.createElement("td");
        td_end_date.innerHTML = formatDate(image["end_date"]);
        tr.appendChild(td_end_date);

        var td_iterations = document.createElement("td");
        td_iterations.innerHTML = image["iterations"];
        tr.appendChild(td_iterations);

        var td_reconstruction_time = document.createElement("td");
        td_reconstruction_time.innerHTML = image["reconstruction_time"];
        tr.appendChild(td_reconstruction_time);

        var td_img = document.createElement("td");
        var img = document.createElement("img");
        img.src = "data:image/png;base64, " + image["img"];
        td_img.appendChild(img);
        tr.appendChild(td_img);
    }

}

function formatDate(date) {
    return new Date(date).toLocaleString();
}