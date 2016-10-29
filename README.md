J1772

This framework is supposed to provide basic functions for J1772 protocol when working with charging communcation controllers for electric vehicles.

#Challenges
In order to provide extreme safety, there are a few circumstances, where special instructions are required to prevent harm.

When the EVSE is at rest (+12V, no PWM), and a person touches the contacts, the positive voltage will drop and commence the PWM phase. When 
