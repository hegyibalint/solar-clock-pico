// switch (phase)
// {
// case 0:
//   digitalWrite(stepper->ulp, HIGH);
//   digitalWrite(stepper->ulm, LOW);
//   digitalWrite(stepper->urp, LOW);
//   digitalWrite(stepper->urm, HIGH);
//   break;
// case 1:
//   digitalWrite(stepper->ulp, LOW);
//   digitalWrite(stepper->ulm, HIGH);
//   digitalWrite(stepper->urp, LOW);
//   digitalWrite(stepper->urm, HIGH);
//   break;
// case 2:
//   digitalWrite(stepper->ulp, LOW);
//   digitalWrite(stepper->ulm, HIGH);
//   digitalWrite(stepper->urp, HIGH);
//   digitalWrite(stepper->urm, LOW);
//   break;
// case 3:
//   digitalWrite(stepper->ulp, HIGH);
//   digitalWrite(stepper->ulm, LOW);
//   digitalWrite(stepper->urp, HIGH);
//   digitalWrite(stepper->urm, LOW);
//   break;
// default:
//   Serial1.println("Invalid phase");
//   break;
// }