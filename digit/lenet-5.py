import math
import numpy as np
import torch
from torch import nn
from torch.nn import functional as F
from torch.utils.data import TensorDataset, DataLoader
from torchvision import transforms

BATCHSIZE = 256
kwargs = {'num_workers': 2, 'pin_memory': True}                              #DataLoader的参数


def initData():
    with open("train.csv", "r") as f:
        lines = f.readlines()
    trainLabels = []
    trainImages = []
    for line in lines[1:]:
        tmp = line.strip().split(",")
        trainLabels.append(int(tmp[0]))
        trainImages.append([int(x) for x in tmp[1:]])

    testImages = []
    with open("test.csv", "r") as f:
        lines = f.readlines()
    testImages = []
    for line in lines[1:]:
        tmp = line.strip().split(",")
        testImages.append([int(x) for x in tmp])

    return trainLabels, trainImages, testImages


class LeNet5(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 6, 5, padding=2)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1 = nn.Linear(16*5*5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
        x = F.max_pool2d(F.relu(self.conv2(x)), (2, 2))
        x = x.view(-1, 400)
        # print('size', x.size())
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


def initWeight(m):
    if isinstance(m, nn.Conv2d):
        n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
        m.weight.data.normal_(0, math.sqrt(2. / n))
    elif isinstance(m, nn.BatchNorm2d):
        m.weigth.data.fill_(1)
        m.bias.data.zero_()


def train(model, criterion, optimizer, trainLoader, epoch):
    model.train()
    for batch_idx, (data, target) in enumerate(trainLoader):
        data, target = data.cuda(), target.cuda()
        optimizer.zero_grad()
        output = model(data)
        loss = criterion(output, target)
        loss.backward()
        optimizer.step()
    print('Train Epoch: {}\tLoss: {:.6f}'.format(epoch, loss.item()))


def valid(model, criterion, validLoader, epoch):
    model.eval()
    test_loss = 0
    correct = 0
    for data, target in validLoader:
        data, target = data.cuda(), target.cuda()
        with torch.no_grad():
            output = model(data)
            test_loss += criterion(output, target).item()
            pred = output.data.max(1, keepdim=True)[1]
            correct += pred.eq(target.data.view_as(pred)).cpu().sum()

    test_loss /= len(validLoader.dataset)
    print('\nTest set: Average loss: {:.4f}, Accuracy: {}/{} ({:.2f}%)\n'.format(
       test_loss, correct, len(validLoader.dataset),
       100. * correct / len(validLoader.dataset)))

def test(model, testLoader):
    model.eval()
    result = []
    for data in testLoader:
        data = data[0].cuda()
        with torch.no_grad():
            output = model(data)
            pred = output.data.max(1, keepdim=True)[1]
        for x in pred:
            result.append(x.item())

    id = 0
    text = "ImageId,Label\n"
    for x in result:
        id += 1
        text += str(id) + "," + str(x) + "\n"

    with open("submit-cnn.csv", "w") as f:
        f.write(text)

if __name__ == "__main__":
    print(LeNet5())
    dataLabels, dataImages, testImages = initData()
    trainLabels = dataLabels[:40000]
    trainImages = dataImages[:40000]
    validLabels = dataLabels[40000:]
    validImages = dataImages[40000:]

    trainX = torch.from_numpy(np.array(trainImages).reshape(-1, 1, 28, 28)).float()
    trainY = torch.from_numpy(np.array(trainLabels).astype(int))
    validX = torch.from_numpy(np.array(validImages).reshape(-1, 1, 28, 28)).float()
    validY = torch.from_numpy(np.array(validLabels).astype(int))
    testX = torch.from_numpy(np.array(testImages).reshape(-1, 1, 28, 28)).float()

    trainDataset = TensorDataset(trainX, trainY)
    validDataset = TensorDataset(validX, validY)
    testDataset = TensorDataset(testX)

    trainLoader = DataLoader(dataset=trainDataset, shuffle=True, batch_size=BATCHSIZE, **kwargs)
    validLoader = DataLoader(dataset=validDataset, shuffle=True, batch_size=BATCHSIZE, **kwargs)
    testLoader = DataLoader(dataset=testDataset, shuffle=False, batch_size=BATCHSIZE, **kwargs)

    model = LeNet5().cuda()

    criterion = nn.CrossEntropyLoss(size_average=False)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001, betas=(0.9, 0.99))

    model.apply(initWeight)

    for epoch in range(128):
        train(model, criterion, optimizer, trainLoader, epoch)
        valid(model, criterion, validLoader, epoch)
    
    test(model, testLoader)