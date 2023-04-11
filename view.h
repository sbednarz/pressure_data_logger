#ifndef VIEW_H_
#define VIEW_H_

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta charset="utf-8">
        <style>
            html {
                font-family: Arial;
                margin: 0px auto;
                text-align: center;
            }
            h5 {
                font-size: 1em;
                color: #888888;
            }
            h3 {
                font-size: 2em;
                margin: 0 0;
            }
            h1 {
                font-size: 3em;
                margin: 0 0;
            }
            .btn1 {
                background-color: #4CAF50; /* Green */
                border: none;
                color: white;
                padding: 15px 32px;
                text-align: center;
                text-decoration: none;
                display: inline-block;
                font-size: 16px;
                margin: 4px 2px;
                cursor: pointer;
            }

            .btn2 {
                background-color: #ffffff;
                color: #808080;
                border-style: solid;
                border-width: 1px;
                border-color: #808080;
                padding: 15px 32px;
                text-align: center;
                text-decoration: none;
                display: inline-block;
                font-size: 16px;
                margin: 4px 2px;
                cursor: pointer;
            }
            .block {
                padding: 8px 8px;
            }
        </style>
    </head>




    <body>
        <div id="block1">
        <h5><span id="info">SENSOR #1</span></h5>
        </div>

        <div id="block2">
            <h3>Czas</h3>
            <h1><span id="time">0</span> s</h1>
            <h5></h5>
        </div>

        <div id="block3">
        <h3>Ciśnienie</h3>
        <h1><span id="pressure">0.00</span> Pa</h1>
        </div>

        <h5><a href="#" class="btn1" id="run" onclick="btn_click()">Start</a></h5>
        
        <div id="block4">
        </div>
    </body>

<script>




function refresh_data() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            const mydata = this.responseText.split(" ");
            document.getElementById("time").innerHTML = mydata[0];
            document.getElementById("pressure").innerHTML = mydata[1];
            if (mydata[2]) {      
                document.getElementById("info").innerHTML = "Trwa pomiar. Wyniki w pliku: "+mydata[2];
            } else {
                document.getElementById("info").innerHTML = "SENSOR#1 gotów.";
            }

        }
    };
    xhttp.open("GET", "/data", true);
    xhttp.send();
}

function set_gui_idle() {
    document.getElementById("block2").style.display = "none";
    document.getElementById("block4").style.display = "block";
    document.getElementById("run").innerHTML = "Start pomiaru";
    document.getElementById("info").innerHTML = "SENSOR#1 gotów.";
}

function set_gui_working() {
    document.getElementById("block2").style.display = "block";
    document.getElementById("block4").style.display = "none";
    document.getElementById("run").innerHTML = "Stop";
    document.getElementById("info").innerHTML = "Trwa pomiar. Zapis wyników do pliku:";
}


function refresh_gui() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/status", true);
    xhttp.send();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText === 'STATUS_IDLE') {
                set_gui_idle();
                refresh_data();
            }
            if (this.responseText === 'STATUS_WORKING') {
                set_gui_working();
                refresh_data();
            }
        }
    };
}



function btn_click() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText === 'STATUS_IDLE') {
                var xhttp = new XMLHttpRequest();
                var t = Date.now();
                var str = "/start?timestamp=" + t;
                xhttp.open("GET", str, true);
                xhttp.send();
                set_gui_working();
            }
            if (this.responseText === 'STATUS_WORKING') {
                var xhttp = new XMLHttpRequest();
                xhttp.open("GET", "/stop", true);
                xhttp.send();
                set_gui_idle();
                refresh_filelist();
            }
        }
    };
    xhttp.open("GET", "/status", true);
    xhttp.send();
}

function refresh_filelist() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            files = this.responseText.split(/\r?\n/);
            var content = "<h5>Wyniki pomiarów</h5>\n"
            for (var i=files.length-2; i>-1; i--) {
                row = files[i].split(' ');
                var t =  new Date(parseInt(row[1]));
                var datetime = t.getFullYear()+"-"+("0"+(t.getMonth()+1)).slice(-2)+"-"+("0"+t.getDate()).slice(-2)+" "
                    +("0"+t.getHours()).slice(-2)+":"+("0"+t.getMinutes()).slice(-2)+":"+("0"+t.getSeconds()).slice(-2);

                content = content + `<h5><a href='/download?filename=${row[0]}' class="btn2">${row[0].substring(1)}<br/>${datetime}</a></h5>\n`;
            }
            document.getElementById("block4").innerHTML = content;
        }
    };
    xhttp.open("GET", "/list", true);
    xhttp.send();
}

function refresh_page() {
    refresh_gui();
    refresh_filelist();
}

setInterval(refresh_data,1000);
document.addEventListener('DOMContentLoaded', refresh_page);

</script>

</html>
)rawliteral";

#endif 
  
