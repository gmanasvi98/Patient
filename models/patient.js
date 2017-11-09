const mongoose = require('mongoose');

const patientSchema = mongoose.Schema({
    patient_name: {
        type: String,
        required: true
    },
    patient_id: {
        type: String,
        required: true
    },
    patient_gender: {
        type: String,
        required: true
    },
    patient_heartbeat: {
        type: String
    },
    patient_temp: {
        type: String
    },
    patient_status: {
        type: String
    },
    patient_location: {
        type: String
    },
    patient_room_temp: {
        type: String
    },
    patient_phone_no: {
        type: String
    }
});

const Patients = module.exports = mongoose.model('patient_data', patientSchema);


module.exports.getPatientAll = function(callback){
    Patients.find({}, callback);
}

module.exports.getPatientByNo = function(patient_no, callback){
    const query = {patient_id:patient_no};
    Patients.findOne(query, callback);
}

module.exports.addPatient = function(newPatient, callback){
    newPatient.save(callback);
}

module.exports.updatePatient = function(id,newPost, callback){
    const query = {patient_id:id};
    Patients.findOneAndUpdate(query, newPost, callback);
}