const mongoose = require('mongoose');

const patientSchema = mongoose.Schema({
    patient_name: {
        type: String,
        required: true
    },
    patient_no: {
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
    }
});

const Patients = module.exports = mongoose.model('patient_data', patientSchema);


module.exports.getPatientAll = function(callback){
    Patients.find({}, callback);
}

module.exports.getPatientByNo = function(patient_no, callback){
    const query = {patient_no:patient_no};
    Patients.findOne(query, callback);
}

module.exports.addPatient = function(newPatient, callback){
    newPatient.save(callback);
}