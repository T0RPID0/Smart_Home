setInterval(function () {
    fetch('/api/v1/update')
        .then(
            function (response) {
                if (response.status !== 200) {
                    console.log('Looks like there was a problem. Status Code: ' +
                        response.status);
                    return;
                }
                // Examine the text in the response
                response.json().then(function (data) {
                    // sync status of all buttons
                    var nodes = document.getElementsByClassName("nodes");
                    for (i = 0; i < nodes.length; i++) {
                        var pins = nodes[i].getElementsByTagName("input");
                        for (j = 0; j < pins.length; j++) {
                            status = data['nodes'][i][pins[j].id];
                            if (status == 'on')
                                pins[j].checked = true;
                            else
                                pins[j].checked = false;
                        }
                    }
                });
            }
        )
        .catch(function (err) {
            console.log('Fetch Error :-S', err);
        });
}, 500);


function check_input(ele) {
    data = {
        node: ele.parentNode.parentNode.id,
        pin: ele.id,
        status: ""
    };
    if (ele.checked) {
        data["status"] = "on";
        send_req("POST", data, ele);
    } else {
        data["status"] = "off"
        send_req("POST", data, ele);
        console.log('Not checked');
    }
}

function send_req(method, data = "", ele) {
    if (method == "POST") {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                postresponse = this.responseText;
                changebtn(postresponse, ele);
            }
        };
        xhttp.open("POST", "/api/v1/update", true);
        xhttp.setRequestHeader('Content-Type', 'application/json');
        xhttp.send(JSON.stringify(data));
    }
    if (method == "GET") {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                getresponse = this.responseText;
                changebtn(getresponse, ele)
            }
        };
        xhttp.open("GET", "/api/v1/update", true);
        xhttp.send();
    }
}

function changebtn(res, ele) {
    if (res == "on") {
        ele.checked = true;
    }
    if (res == "off") {
        ele.checked = false;
    }
    if (res == "unreachable") {
        console.log('device not found!')
        ele.checked = false;
    }
}

//set camera
function setcam(ele) {
    live_feed=document.getElementById("live_feed")
    live_feed.setAttribute("src","/video_feed/"+ele.id)
}