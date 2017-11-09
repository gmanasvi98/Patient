const express = require('express');
const router = express.Router();
const Patients = require('../models/patient');


router.get('/',(req,res,next)=>{
    res.json({"success":"done"});
})

router.post('/patient/new', (req, res, next)=>{
    let newPatient =new  Patients({
        patient_name: req.body.patient_name,
        patient_id: req.body.patient_id,
        patient_gender:req.body.patient_gender,
        patient_heartbeat: "",
        patient_temp: "",
        patient_status: "",
        patient_location: "",
        patient_room_temp: "",
        patient_phone_no: req.body.patient_phone_no
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
            res.json({"results":patients});
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

router.post('/patients/patient/:id', (req, res, next)=>{
    let id = req.params.id;
    let newPost ={$set:{
        patient_heartbeat: req.body.patient_heartbeat || "",
        patient_temp: req.body.patient_temp || "",
        patient_status: req.body.patient_status || "",
        patient_location: req.body.patient_location || "",
        patient_room_temp: req.body.patient_room_temp || "",
    }};
    Patients.updatePatient(id, newPost, (err, post)=>{
        if (err) {
            res.json({success:false, msg:"Failed to update patient "});
            console.log(err);
        } else {
            res.json({success:true, msg:"Updated post patient"});
        }
    })

});



module.exports = router;