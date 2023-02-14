
window.setInterval(function() {
    getAllNewDevices();
}, 3000);

window.addEventListener('load', (e) => {
    getOnOffDevices()
});

if((savedSensor() == null || savedSensor().type !=="Light sensor")){
    document.getElementById('sensorLimits').hidden=true ;
}


var selectedDevice;
function getOnOffDevices(){
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
                 if ((savedSensor().type ==="button" || savedSensor().type ==="Light sensor")
                                                    && device.type === "lightBulb") {
                     html += `
                    <div id ="${device.id}" class="device stOff col-xs-1 col-sm-3 col-md-2 ">
                        <p id ="select"></p>
                        <h5>${device.name}</h5>
                        <i class="far fa-lightbulb fa-7x"></i>
                        <p>${device.type}</p>
                    </div>
                `;

                 }else if (savedSensor().type ==="Shutter Button" && device.type === "shutter"){
                     html += `
                    <div id ="${device.id}" class="device stOff col-xs-1 col-sm-3 col-md-2 ">
                        <p id ="select"></p>
                        <h5>${device.name}</h5>
                        <i class="far fa-window-maximize  fa-7x"></i>
                        <p>${device.type}</p>
                    </div>
                `;

                 }
            })


            document.getElementById('allDevices').innerHTML =html;
        })
        .catch(error => console.log('error', error));
}

$(document).on('click','.device', function () {
    if(selectedDevice != null){
        selectedDevice.style.backgroundColor="#145374";
        selectedDevice.childNodes[1].innerHTML = ""
    }
    selectedDevice = $(this).attr('id');
    selectedDevice = document.getElementById(selectedDevice)
    selectedDevice.style.backgroundColor="#595959";
    selectedDevice.childNodes[1].innerHTML = "SELECTED"
});

function saveNewSensor() {
    var myHeaders = new Headers();
    myHeaders.append("Content-Type", "application/json");



    var requestOptions = {
        method: 'POST',
        headers: myHeaders,
        redirect: 'follow'
    };

    var ulrParams ="";
    if(savedSensor().type==="Light sensor"){
        var lower= document.getElementById('lower').value;
        var upper= document.getElementById('upper').value;
        ulrParams=url+"domoApi/devices/addSensor/"+savedSensor().name+"?controlledDeviceId="+selectedDevice.id+
            "&lowerLimit="+lower+"&upperLimit="+upper;

    }else{
        ulrParams=url+"domoApi/devices/addSensor/"+savedSensor().name+"?controlledDeviceId="+selectedDevice.id;
    }

     fetch(ulrParams, requestOptions)
         .then(function (response){
             if(response.ok){
                 window.location = "index.html"
             }
         })
         .then(function (data){
         })
        .catch(error => console.log('error',error));


}


