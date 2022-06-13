import os

import torch
import torch.nn as nn


class VAE(nn.Module):
    def __init__(self, num_channals, latent_dim):
        super(VAE, self).__init__()
        self.num_channals = num_channals
        self.latent_dim = latent_dim
        # Define the architecture of VAE here
        # TODO START
        self.encoder = nn.Sequential(
            nn.Conv2d(num_channals, 16, kernel_size=3, padding=1),
            nn.ReLU(True),
            nn.Conv2d(16, 32, kernel_size=3, stride=2, padding=1),
            nn.ReLU(True),
            nn.Conv2d(32, 32, kernel_size=3, padding=1),
            nn.ReLU(True),
            nn.Conv2d(32, 16, kernel_size=3, stride=2, padding=1),
            nn.ReLU(True),
            nn.Flatten(),
            nn.Linear(1024, 2 * latent_dim),
            nn.ReLU(True),
            nn.Linear(2 * latent_dim, 2 * latent_dim)
        )
        self.decoder1 = nn.Sequential(
            nn.Linear(latent_dim, latent_dim),
            nn.ReLU(True),
            nn.Linear(latent_dim, 1024),
        )   # reshape: (-1, 16, 8, 8)
        self.decoder2 = nn.Sequential(
            nn.ConvTranspose2d(16, 32, kernel_size=3, stride=2, padding=1, output_padding=1),
            nn.ReLU(True),  # dec-3
            nn.ConvTranspose2d(32, 32, kernel_size=3, padding=1),
            nn.ReLU(True),  # dec-4
            nn.ConvTranspose2d(32, 16, kernel_size=3, stride=2, padding=1, output_padding=1),
            nn.ReLU(True),  # dec-5
            nn.ConvTranspose2d(16, num_channals, kernel_size=3, padding=1),
            nn.Sigmoid(),   # dec-6
        )
        # TODO END

    def reparameterize(self, mu, log_var):
        '''
        *   Arguments:
            *   mu (torch.FloatTensor): [batch_size, latent_dim], parameters of the diagnoal Gaussian posterior q(z|x)
            *   log_var (torch.FloatTensor): [batch_size, latent_dim], parameters of the diagnoal Gaussian posterior q(z|x)
        *   Returns:
            *   reparameterized samples (torch.FloatTensor): [batch_size, latent_dim]
        '''
        # TODO START
        epsilon = torch.randn_like(mu)
        sampled_z = epsilon * torch.exp(0.5 * log_var) + mu
        return sampled_z
        # TODO END

    def forward(self, x=None, z=None):
        '''
        *   Arguments:
            *   x (torch.FloatTensor): [batch_size, 1, 32, 32]
            *   z (torch.FloatTensor): [batch_size, latent_dim]
        *   Returns:
            *   if `x` is not `None`, return a list:
                *   Reconstruction of `x` (torch.FloatTensor)
                *   mu (torch.FloatTensor): [batch_size, latent_dim], parameters of the diagnoal Gaussian posterior q(z|x)
                *   log_var (torch.FloatTensor): [batch_size, latent_dim], parameters of the diagnoal Gaussian posterior q(z|x)
            *  if `x` is `None`, return samples generated from the given `z` (torch.FloatTensor): [num_samples, 1, 32, 32]
        '''
        if x is not None:
            # TODO START
            output = self.encoder(x)
            mu, log_var = output.split(self.latent_dim, dim=-1)
            z = self.reparameterize(mu, log_var)
            recon_x = self.forward(z=z)
            return recon_x, mu, log_var
            # TODO END
        else:
            assert z is not None
            # TODO START
            gen_x = self.decoder2(self.decoder1(z).view(-1, 16, 8, 8))
            return gen_x
            # TODO END

    def restore(self, ckpt_dir):
        try:
            if os.path.exists(os.path.join(ckpt_dir, 'pytorch_model.bin')):
                path = os.path.join(ckpt_dir, 'pytorch_model.bin')
            else:
                path = os.path.join(ckpt_dir, str(max(int(name)
                                                      for name in os.listdir(ckpt_dir))), 'pytorch_model.bin')
        except BaseException:
            return None
        self.load_state_dict(torch.load(path))
        return os.path.split(path)[0]

    def save(self, ckpt_dir, global_step):
        os.makedirs(os.path.join(ckpt_dir, str(global_step)), exist_ok=True)
        path = os.path.join(ckpt_dir, str(global_step), 'pytorch_model.bin')
        torch.save(self.state_dict(), path)
        return os.path.split(path)[0]
