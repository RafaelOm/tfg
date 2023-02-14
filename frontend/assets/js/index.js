window.setInterval(function() {
    getAllDevices();
}, 500);

window.setInterval(function() {
    getAllNewDevices();
}, 3000);

//getAllDevices();
$(document).on('click','.fa-lightbulb', function () {
    var myHeaders = new Headers();
    myHeaders.append("Content-Type", "application/json");

    let id =$(this).parent().attr('id');
    console.log(id)
    let status =changeDeviceStatus(id);
    var requestOptions = {
        method: 'POST',
        headers: myHeaders,
        redirect: 'follow'
    };

    console.log("domoApi/devices/setStatus/"+id+"/"+status);
    fetch(url+"domoApi/devices/setStatus/"+id+"/"+status, requestOptions)
        .then(response => response.text())
        .then(result => console.log(result))
        .catch(error => console.log('error', error));

});

$(document).on('click','.fa-chevron-up', function () {
    let id =$(this).parent().attr('id');
    setMovementShutter(id,"Up");

});
$(document).on('click','.fa-chevron-down', function () {
    let id =$(this).parent().attr('id');
    setMovementShutter(id,"Down");

});
$(document).on('click','.fa-stop-circle', function () {
    let id =$(this).parent().attr('id');
    setMovementShutter(id,"Stop");

});
$(document).on('click','.saveAutoDevice', function () {
    let id =$(this).parent().attr('id');
    console.log("hola")
    addAutoDevice(id);

});
$(document).on('click','.config', function () {
    let id =$(this).parent().attr('id');
    actualModifyingDeviceId =id;
    console.log("hola")
    fillConfigModal(id);

});
$(document).on('click','.saveConfig', function () {
    console.log("hola")
    modifyDevice(actualModifyingDeviceId);

});


$(document).on('click','.removeDevice', function () {
    deleteDevice(actualModifyingDeviceId);

});