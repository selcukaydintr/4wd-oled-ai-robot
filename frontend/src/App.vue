<template>
  <div id="app">
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
      <div class="container-fluid">
        <router-link class="navbar-brand" to="/">
          <i class="bi bi-robot"></i> Robot Control Panel
        </router-link>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
          <ul class="navbar-nav">
            <li class="nav-item">
              <router-link class="nav-link" to="/plugins">
                <i class="bi bi-puzzle"></i> Plugins
              </router-link>
            </li>
            <li class="nav-item">
              <router-link class="nav-link" to="/slam">
                <i class="bi bi-map"></i> SLAM
              </router-link>
            </li>
            <li class="nav-item">
              <router-link class="nav-link" to="/ml">
                <i class="bi bi-cpu"></i> Machine Learning
              </router-link>
            </li>
          </ul>
        </div>
      </div>
    </nav>
    
    <div class="container-fluid mt-3">
      <router-view></router-view>
    </div>
  </div>
</template>

<script>
export default {
  name: 'App'
}
</script>

<style>
@import url("https://cdn.jsdelivr.net/npm/bootstrap-icons@1.11.1/font/bootstrap-icons.css");

.navbar {
  box-shadow: 0 2px 4px rgba(0,0,0,.1);
}

.nav-link {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.nav-link i {
  font-size: 1.2rem;
}

.router-link-active {
  color: #fff !important;
  background-color: rgba(255,255,255,.1);
  border-radius: 4px;
}
</style> 