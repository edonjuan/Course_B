#include "painlessMesh.h"

#define   MESH_PREFIX     "UTEQ-INTEL"
#define   MESH_PASSWORD   "UTEQ-INTEL"
#define   MESH_PORT       5555
#define   MASTER          693462094
#define   YOUR_NAME       "Eduardo Donjuan"     // <-- Change to your name!

Scheduler userScheduler;
painlessMesh  mesh;

void sendMessage();
Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello i'm ";
  msg += YOUR_NAME;
  msg += " from node: ";
  msg += mesh.getNodeId();
  mesh.sendSingle(MASTER, msg);
  taskSendMessage.setInterval( random( TASK_SECOND * 10, TASK_SECOND * 15 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // change led state
}

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);

  //mesh.setDebugMsgTypes( ERROR | STARTUP );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);

  Serial.println();
  Serial.print("MI ID ES: ");
  Serial.println(mesh.getNodeId());
  
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  mesh.update();
}
