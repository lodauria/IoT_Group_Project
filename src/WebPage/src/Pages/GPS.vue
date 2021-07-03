<template>
  <div>
    <b-form action="#" @submit="onSubmit">
      <b-form-group
          class="my-3"
          label-for="plate"
          label="Boat license plate"
          description="Use the pattern AB1234"
      >
        <b-form-input
            id="plate"
            placeholder="AB1234"
            v-model="form.plate"
            pattern="[A-Za-z]{2}[0-9]{2,6}"
            required>
        </b-form-input>
      </b-form-group>
      <div class="my-2">
        <b-button type="submit" variant="success" :disabled="sendingUpdate">Enable</b-button>
        <b-button class="mx-3" variant="danger" v-on:click="disableGPS" :disabled="!sendingUpdate">Disable</b-button>
      </div>
    </b-form>
    <div class="text-center" v-if="sendingUpdate">
      <h3 class="text-success">Sending device position...</h3>
    </div>
    <div class="text-center" v-if="boatDetected">
      <h3 class="text-success">Boat detected</h3>
      <p>Follow the smart signage instructions</p>
      <p>You can close the web page</p>
    </div>
  </div>
</template>

<script>
import axios from "axios";

export default {
  name: "GPS",
  data() {
    return {
      form: {
        plate: '',
      },
      watchId: null,
      sendingUpdate: false,
      boatDetected: false
    }
  },
  mounted() {
    if (!("geolocation" in navigator)) {
      const errorStr = 'Geolocation is not available.';
      alert(errorStr)
      return;
    }
  },
  methods: {
    onSubmit(event) {
      event.preventDefault(); //Used to avoid page reload

      const options = {
        enableHighAccuracy: true,
        maximumAge: 1000
      }

      this.sendingUpdate = false

      this.watchId = navigator.geolocation.watchPosition(
          pos => {
            this.sendGPSUpdate(pos.coords.latitude, pos.coords.longitude)
          },
          err => {
            console.log(err.message)
            this.sendingUpdate = false
          },
          options)

      return false;
    },
    disableGPS() {
      if (this.watchId) {
        navigator.geolocation.clearWatch(this.watchId);
        this.watchId = null
        this.sendingUpdate = false
      }
    },

    sendGPSUpdate(lat, long) {
      console.log("POS", lat, long)
      const data = {}
      const headers = {
        headers: {
          "boat-id": this.form.plate,
          "latitude": Number(lat),
          "longitude": Number(long)
        }
      }
      axios.post("https://kayib6g5eg.execute-api.us-east-1.amazonaws.com/default/GPSHandle", data, headers).then(d => {
        if(d.data.ret_code==0){
          this.boatDetected = true
          this.disableGPS();
        }
        else {
          this.sendingUpdate = true
          this.boatDetected = false
        }
      }).catch(e => {
        console.log(e)
      })
    }
  }
}
</script>

<style scoped>

</style>