#!/usr/bin/env python
import os
import time
import numpy as np
import lo
import siddon
from siddon.solar import read_data
# data
obsrvtry = ('STEREO_A', 'STEREO_B')
data = siddon.solar.concatenate(
    [read_data(os.path.join(os.getenv('HOME'), 'data', 'siddon', '171dec08'), 
               bin_factor=16.,
               obsrvtry=obs,
               time_window=['2008-12-01T00:00:00.000', 
                            '2008-12-15T00:00:00.000'],
               time_step=16 * 3600.
               )
     for obs in obsrvtry])
data = siddon.solar.sort_data_array(data)
# scale A and B images
# the ratio of sensitivity between EUVI A and B
calibration_ba = {171:0.902023, 195:0.974536, 284:0.958269, 304:1.05954}
for i in xrange(data.shape[-1]):
    if data.header['OBSRVTRY'][i] == 'STEREO_B':
        data[..., i] /= calibration_ba[data.header['WAVELNTH'][i]]

# make sure it is 64 bits data
data.header['BITPIX'][:] = -64
# cube
shape = np.asarray(3 * (64.,))
crpix = shape / 2.
cdelt = 3. / shape
header = {'CRPIX1':crpix[0], 'CRPIX2':crpix[1], 'CRPIX3':crpix[2],
          'CDELT1':cdelt[0], 'CDELT2':cdelt[1], 'CDELT3':cdelt[2],
          'CRVAL1':0., 'CRVAL2':0., 'CRVAL3':0.,}
cube = siddon.fa.zeros(shape, header=header)
# model
kwargs = {'obj_rmin':1., 'obj_rmax':1.5, 'data_rmin':0.66, 'data_rmax':1.1,
          'mask_negative':False}
P, D, obj_mask, data_mask = siddon.models.srt(data, cube, **kwargs)
# hyperparameters
hypers = cube.ndim * (1e-1, )
# inversion
# expected time
b = data.flatten()
t = time.time()
bpj = P.T * data.flatten()
print((time.time() - t) * 4 * 100 )
# real time
t = time.time()
#b = data[data_mask == 0]
b = data.flatten()
#sol = lo.quadratic_optimization(P, b, D, hypers, maxiter=100)
sol = lo.acg(P, b, D, hypers, maxiter=100)
print(time.time() - t)
# reshape result
fsol = siddon.fa.asfitsarray(sol.reshape(cube.shape), header=header)
fsol.tofits(os.path.join(siddon_path, "output", "test_siddon_secchi_mask.fits"))
