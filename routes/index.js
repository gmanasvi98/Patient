const express = require('express');
const router = express.Router();
const Patients = require('../models/patient');


router.get('/',(req,res,next)=>{
    res.json({"success":"done"});
})

router.post('/patient/new', (req, res, next)=>{
    let newPatient =new  Patients({
        patient_name: req.body.driver_name,
        patient_no: req.body.driver_uid,
        patient_heartbeat: req.body.driver_uid,
        patient_temp: req.body.driver_uid,
        patient_status: req.body.driver_uid,
        patient_location: req.body.driver_uid,
        patient_room_temp: req.body.driver_uid
    });
    Patients.addPatient(newPatient,(err,driver)=>{
        if (err) {
            res.json({success:false, msg:"Failed to add the patient in database"});
            console.log(err);
        } else {
            res.json({success:true, msg:"Patient added succefully"});
        }
    })
});

router.get('/patients/all', (req, res, next)=>{
    Patients.getPatientAll((err, patients)=>{
        if (err) {
            res.json({"error":"error"});
            console.log(err)
        } else {
            res.json(patients);
        }
    })
});

router.get('/patients/patient/:id', (req, res, next)=>{
    let id = req.params.id;
    Patients.getPatientByNo(id, (err, patient)=>{
        if (err) {
            res.json({"error":"error"});
        } else {
            res.json(patient);
        }
    })
});



module.exports = router;