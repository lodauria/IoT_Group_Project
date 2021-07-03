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
        <b-button type="submit" variant="success">Enable</b-button>
        <b-button class="mx-3" variant="danger" v-on:click="disableGPS">Disable</b-button>
      </div>
    </b-form>
  </div>
</template>

<script>
export default {
  name: "GPS",
  data() {
    return {
      form: {
        plate: '',
      },
      watchId: null,
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
    onSubmit() {

      this.watchId = navigator.geolocation.watchPosition(pos => {
        this.sendGPSUpdate(pos.coords.latitude, pos.coords.longitude)
      }, err => {
        console.log(err.message)
      })
      return false;
    },
    disableGPS() {
      if (this.watchId)
        navigator.geolocation.clearWatch(this.watchId);
    },
    sendGPSUpdate(lat, long) {
      console.log("POS",lat,long)
    }
  }
}
</script>

<style scoped>
.checkboxText {
  padding-left: 10px;
}
</style>