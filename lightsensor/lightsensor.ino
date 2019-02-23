
constexpr int LIGHT_SENSOR_PORT = A5;
void setup() {
  Serial.begin(9600);

}

template<typename T,size_t N>
struct array
{
  T data[N];
  array(){}
  T& operator[](size_t n) const
  {
    return data[n];
  }
}

array<short,8> lightings()
{
  array<short,8> ret;
  
}

unsigned int count=0;
unsigned int accum=0;
void loop() {
  auto light_value = analogRead(A5);
  accum+=light_value;
  ++count;
  if(count==100)
  {
    Serial.println(accum/100);
    accum=0;
    count=0;
  }
}
