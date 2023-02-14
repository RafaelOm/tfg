
const url ="http://localhost:8080/";
var actualModifyingDeviceId;




function changeDeviceStatus(id){
    console.log(document.getElementById(id).classList);
    if(document.getElementById(id).classList.contains("on")){
        return "off";
    }else{
        return "on";
    }

}

function getAllDevices(){

    var requestOptions = {
        method: 'GET',
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices", requestOptions)
        .then(function (response){
            return response.json();
        })
        .then(function (data){
            let html='';
            data.forEach(function (device){
            if(device.type ==='shutter'){
                html +=
                    `<div id =${device.id} class="device col-xs-11 col-sm-5 col-md-3 ">
                        <h5>${device.name}</h5>
                        <i class="far fa-window-maximize fa-7x"></i>
                        <p>Shutter</p>

                        <div class="progress">
                            <div class="progress-bar progress-bar-striped bg-info" role="progressbar" style="width: ${device.status}%" aria-valuenow="${device.status}" aria-valuemin="0" aria-valuemax="100">${device.status}%</div>
                        </div>
        
                        <i class="fas fa-chevron-up fa-2x"></i>
                        <i class="fas fa-chevron-down fa-2x"></i>
                        <i class="fas fa-stop-circle fa-2x"></i>
                        <i class="fas fa-cog fa-2x config" data-bs-toggle="modal" href="#modifyDevice"></i>
                    </div>`;


            }else if (device.type==="lightBulb"){
                    html +=`
                 <div id =${device.id} class="device ${device.status} col-xs-11 col-sm-5 col-md-3 ">
                    <h5>${device.name}</h5>
                    <i class="far fa-lightbulb ${device.status} fa-7x"></i>
                    <p>LightBulb</p>
                    <a>${device.status}</a><br>
                        <i class="fas fa-cog fa-2x config" data-bs-toggle="modal" href="#modifyDevice"></i>

                   </div>
                       
                `;

            }else if(device.type ==="button"){
                html +=`
                <div id =${device.id} class="device stOff col-xs-11 col-sm-5 col-md-3 ">
                    <h5>${device.name}</h5>
                <i class="fas fa-toggle-on fa-6x"></i>
                <p>Button</p>
                <a>Controlling: ${device.controlledDevice.name} </a><br>
                    <i class="fas fa-cog fa-2x config" data-bs-toggle="modal" href="#modifyDevice"></i>
                </div>
                `;
            }else if (device.type === "Shutter Button"){
                html +=`
                   <div id ="${device.id}" class="device stOff col-xs-11 col-sm-5 col-md-3 ">
                    <h5>${device.name}</h5>
                    <i class="fas fa-chevron-circle-up fa-6x"></i>
                    <i class="fas fa-chevron-circle-down fa-6x"></i>
                    <p>Shutter Button</p>
                    <a>Controlling: ${device.controlledDevice.name}</a><br>
                    <i class="fas fa-cog fa-2x config" data-bs-toggle="modal" href="#modifyDevice"></i>
                   </div>
                `;

            }else if(device.type === "Light sensor"){
                html +=`
                <div id =${device.id} class="device stOff col-xs-11 col-sm-5 col-md-3 ">
                    <h5>${device.name}</h5>
                <i class="fas fa-adjust fa-6x"></i>
                <p>Light Sensor</p>
                <a>Controlling: ${device.controlledDevice.name} </a><br>
                    <i class="fas fa-cog fa-2x config" data-bs-toggle="modal" href="#modifyDevice"></i>
                </div>
                `;
            }
            })


            document.getElementById('allDevices').innerHTML =html;
        })
        .catch(error => console.log('error', error));


}
function getAllNewDevices(){

    var requestOptions = {
        method: 'GET',
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices/findNewDevices", requestOptions)
        .then(function (response){
            return response.json();
        })
        .then(function (data){
            let html='';
            data.forEach(function (device){
                    html +=`
                        <div id =${device.name} class="device on col-xs-12 col-sm-12 col-md-12 ">
                            <h5>${device.name}</h5>
                            <i class="far fa-circle fa-7x"></i>
                            <p>${device.type}</p>
                            <br/>   
                `;
                    if(device.type !== "button" && device.type !== "Shutter Button" && device.type !== "Light sensor"){
                        html +=`
                         <button class="btn btn-primary alert-warning saveAutoDevice">Add Device</button>
                       </div>
                        `;
                    }else{
                        window.localStorage.setItem('newSensor',JSON.stringify(device));
                        html +=`
                         <button class="btn btn-primary alert-warning configSensor" onclick="window.location = 'configSensor.html'">Add & Config Device</button>
                       </div>
                        `;
                    }

            })


            document.getElementById('newDevices').innerHTML =html;
        })
        .catch(error => console.log('error', error));


}
function addDevice(){

    var myHeaders = new Headers();
    myHeaders.append("Content-Type", "application/json");

    var raw = JSON.stringify({
       "name": document.getElementById('deviceName').value,
        "route": document.getElementById('deviceRoute').value,
        "type": document.getElementById('deviceType').value,
        "shutterMoveRoute": document.getElementById('shutterMoveRoute').value
    });

    var requestOptions = {
        method: 'POST',
        headers: myHeaders,
        body: raw,
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices", requestOptions)
        .then(response => response.text())
        .then(result => function (){
        })
        .catch(error => console.log('error',error));

    $('#addDeviceModal').modal('hide');
    document.getElementById('deviceName').value="";
    document.getElementById('deviceRoute').value="";
    document.getElementById('deviceType').value=""
}

function addAutoDevice(device){

    var myHeaders = new Headers();
    myHeaders.append("Content-Type", "application/json");



    var requestOptions = {
        method: 'POST',
        headers: myHeaders,
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices/addAutoDevices/"+device, requestOptions)
        .then(response => response.text())
        .then(result => function (){
        })
        .catch(error => console.log('error',error));

    $('#addDeviceModal').modal('hide');
    document.getElementById('deviceName').value="";
    document.getElementById('deviceRoute').value="";
    document.getElementById('deviceType').value=""
}

function setMovementShutter(id,movementDirection){
    var myHeaders = new Headers();
    myHeaders.append("Content-Type", "application/json");


    var requestOptions = {
        method: 'POST',
        headers: myHeaders,
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices/setMoveShutter/"+id+"/"+movementDirection, requestOptions)
        .then(response => response.text())
        .then(result => console.log(result))
        .catch(error => console.log('error', error));


}

function fillConfigModal(id){

    var requestOptions = {
        method: 'GET',
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices/"+id, requestOptions)
        .then(function (response){
            return response.json();
        })
        .then(function (device){
            //Limpiamos los valores del formulario anteriores
            document.getElementById('deviceNameModify').value="";
            document.getElementById('deviceTypeModify').value ="";
            document.getElementById('deviceRouteModify').value="";
            document.getElementById('shutterMoveRouteModify').value ="";


            document.getElementById('deviceNameModify').value=device.name;
            document.getElementById('deviceTypeModify').value =device.type;
            document.getElementById('deviceRouteModify').value= device.route;
            document.getElementById('shutterMoveRouteModify').value = device.shutterMoveRoute;
        })
        .catch(error => console.log('error', error));



}

function modifyDevice(id){

    var requestOptions = {
        method: 'PUT',
        redirect: 'follow'
    };

    name =document.getElementById('deviceNameModify').value;
    type =document.getElementById('deviceTypeModify').value;
    route =document.getElementById('deviceRouteModify').value;
    shutterMoveRoute =document.getElementById('shutterMoveRouteModify').value;

    fetch(url+"domoApi/devices/"+id+"?name="+name+"&route="+route+"&type="+type+"&shutterMoveRoute="+shutterMoveRoute, requestOptions)
        .then( function (){
            $('#modifyDevice').modal('hide');
        })
        .then(result => console.log(result) )
        .catch(error => console.log('error', error));



}

function deleteDevice(id){
    var requestOptions = {
        method: 'DELETE',
        redirect: 'follow'
    };

    fetch(url+"domoApi/devices/"+id, requestOptions)
        .then( function (){
            $('#modifyDevice').modal('hide');
        })
        .then(result => console.log(result) )
        .catch(error => console.log('error', error));



}
function savedSensor(){
var newSensor = JSON.parse(window.localStorage.getItem('newSensor'))
    return newSensor;
}


