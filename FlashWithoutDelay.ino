int ledPin = LED_BUILTIN;
int ledState = LOW;
unsigned long rangeBottom = 0;
unsigned long currentPointInRange = 0;
long topOfRange_OnTime = 250;
long topOfRange_OffTime = 750;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

/**
  * This logic threw me for a bit of a loop at first (pun always intended), so I'm spelling it out for
  * future me b/c it's important to understand the fundimentals of how this works.
  *
  * What we are doing here is looking for where we are in a specific range that is always moving.
  * In this case we want to create two ranges: 0 to 250 (topOfRange_OnTime) and 0 to 750 (topOfRange_OffTime).
  * We need a way to create these 0 to whatever ranges and determine where we're at within those ranges for any given
  * millisecond.
  *
  * We're using milliseconds for our scale, so to determine the bottom of our range (0) at any given time, we can subtract
  * the current milliseconds value from itself. We don't want to do that _every loop_, so we put that setting in a conditional.
  * This way, when we start the program our floor can be 0 (millis of 0 minus millis of 0 is 0), and when we decide we want to
  * create a new bottom of the range so we can continue using the ever growing millis value to count we can again subtract millis
  * from itself (millis of 250 - millis of 250 is still 0). We only set the bottom when we've hit the top of our range (250 or 750),
  * so we know that whatever our actual millis and bottom values are, we have an artifical bottom of 0.
  *
  * The next thing we need to tackle is the top of our artificial range. That's easy, it's whatever we want it to be. In this
  * case we're looking for either 250 or 750. These values are really triggers. When we hit those triggers we need we need to create
  * a new bottom range and then do whatever other logic we would like triggered (turning on an LED, buzzing a buzzer, etc).
  *
  * With the artifical bottom and an arbitrary top of the range we can use the current milliseconds value minus the current actual bottom
  * value to determine where we are in our artificial range. If the current millisecond value is 15934 and our actual range bottom is
  * currently set to 15695 then we know that our current point in our artifical range of 0 to 250 is 240 (15934 - 15695).
  *
  * As time passes the current millisecond value goes up, but the actual bottom stays the same, so the difference between the
  * two values grows. Once we hit our artifical top of the range we know we need to create a new range so we can set the actual
  * bottom to a new value, the current milliseconds value. On the next iteration of our loop the current point in our range
  * (millis - actual bottom) will equal 0 and our artificial range values start over.
  *
  * The part that determines which artifical range we're using at the moment is the state of the LED. In the conditional we check
  * to see if the LED state is LOW or HIGH. If it's state is LOW, the rest of the first conditional tells us we're using the
  * topOfRange_OffTime value as the top of our artifical range. If the LED state is HIGH we know we're using the topOfRange_OnTime
  * value as the top of our artifical range.
  *
  * And that's it for this example. The milliseconds will always count up and we will create an artifical range to count between 0
  * and an arbitrary value until we hit that value and create a new artifical range. Each time we hit the top of a given range we
  * will do some action.
*/
void loop() {

  unsigned long currentMillis = millis();
  currentPointInRange = currentMillis - rangeBottom;


  // ============================================================================ //
  // Printing the current time/state so we can inspect the current loop logic.
  Serial.println("=====================================");

  Serial.print("current milliseconds since start is: ");
  Serial.println(millis());

  Serial.print("current actual range bottom is: ");
  Serial.println(rangeBottom);

  Serial.println("-----------");

  Serial.print("artifical range bottom (each successful conditional sets this to 0 (millis - self at the top of the loop after a conditional success)) is: ");
  Serial.println(0); // I'm using a 0 here b/c there isn't a good way of calculating it each loop iteration.

  Serial.print("current point in range is: ");
  Serial.println(currentPointInRange);

  if (ledState == LOW) {
    Serial.print("current top of range is: ");
    Serial.println(topOfRange_OffTime);
  } else if (ledState == HIGH) {
    Serial.print("current top of range is: ");
    Serial.println(topOfRange_OnTime);
  }

  Serial.print("the led state is: ");
  Serial.println(ledState);

  Serial.println("=====================================");
  // ============================================================================ //

  // The actual loop logic

  if( (ledState == HIGH) && (currentPointInRange >= topOfRange_OnTime))
  {
    ledState = LOW;
    rangeBottom = currentMillis;
    digitalWrite(ledPin, ledState);
  }
  else if ((ledState == LOW) && currentPointInRange >= topOfRange_OffTime)
  {
    ledState = HIGH;
    rangeBottom = currentMillis;
    digitalWrite(ledPin, ledState);
  }

}