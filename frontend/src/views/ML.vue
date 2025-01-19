<template>
  <div class="multimodal-ml">
    <div class="row">
      <!-- Modalite Kontrolleri -->
      <div class="col-md-3">
        <div class="card">
          <div class="card-header">
            <h5>Modalite Kontrolleri</h5>
          </div>
          <div class="card-body">
            <div class="form-check mb-2">
              <input type="checkbox" class="form-check-input" v-model="modalites.vision">
              <label class="form-check-label">Görüntü İşleme</label>
            </div>
            <div class="form-check mb-2">
              <input type="checkbox" class="form-check-input" v-model="modalites.audio">
              <label class="form-check-label">Ses İşleme</label>
            </div>
            <div class="form-check mb-2">
              <input type="checkbox" class="form-check-input" v-model="modalites.motion">
              <label class="form-check-label">Hareket İşleme</label>
            </div>
          </div>
        </div>
      </div>

      <!-- Canlı Analiz -->
      <div class="col-md-9">
        <div class="card">
          <div class="card-header">
            <h5>Multimodal Analiz</h5>
          </div>
          <div class="card-body">
            <div class="row">
              <!-- Kamera Görüntüsü -->
              <div class="col-md-6">
                <div class="camera-feed">
                  <!-- Kamera komponenti -->
                </div>
              </div>
              
              <!-- Analiz Sonuçları -->
              <div class="col-md-6">
                <div class="analysis-results">
                  <h6>Görüntü Analizi</h6>
                  <div class="progress mb-3">
                    <div :style="{width: visionConfidence + '%'}" 
                         class="progress-bar"></div>
                  </div>
                  
                  <h6>Ses Analizi</h6>
                  <div class="progress mb-3">
                    <div :style="{width: audioConfidence + '%'}" 
                         class="progress-bar"></div>
                  </div>
                  
                  <h6>Hareket Analizi</h6>
                  <div class="progress mb-3">
                    <div :style="{width: motionConfidence + '%'}" 
                         class="progress-bar"></div>
                  </div>
                  
                  <h6>Füzyon Sonucu</h6>
                  <div class="alert alert-info">
                    {{ fusionResult }}
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template> 